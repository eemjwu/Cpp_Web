#include "CSql.h"

CSql::CSql()
{
}


CSql::~CSql()
{
}

int CSql::ListAllFile(std::string& json)
{
	std::vector<std::string> jsonarray;
	WebTool::TSqlData sqlData;
	std::string rSql;
	rSql = "select * from t_file_pic_info where private_attr = '1'";
	SQL.execSQL(rSql, sqlData);
	std::vector<std::string > Head = sqlData.getHead();
	DBG(L_DEBUG, "��ñ�ͷִ��");
	for (int row = 0; row < sqlData.getRow(); row++)
	{
		std::string jsonstr;
		for (int col = 0; col < sqlData.getCol(); col++)
			WebTool::TJson::setStr(jsonstr, Head[col], sqlData.getData(row, col));
		jsonarray.push_back(jsonstr);

	}
	WebTool::TJson::setArray(json, "file_pic_info", jsonarray);
	jsonarray.clear();
	rSql = "select * from t_file_vid_info where private_attr = '1'";
	SQL.execSQL(rSql, sqlData);
	Head = sqlData.getHead();
	DBG(L_DEBUG, "��ñ�ͷִ��");
	for (int row = 0; row < sqlData.getRow(); row++)
	{
		std::string jsonstr;
		for (int col = 0; col < sqlData.getCol(); col++)
			WebTool::TJson::setStr(jsonstr, Head[col], sqlData.getData(row, col));
		jsonarray.push_back(jsonstr);

	}
	WebTool::TJson::setArray(json, "file_vid_info", jsonarray);
	return CSQL_SUCCESS;
}

int CSql::ListFileInfo(const std::string& user_name, std::string &json)
{
	DBG(L_DEBUG, "����listinfo");
	//������֤�Ƿ��¼
	// select online from t_user_info where user_name = 'wmj';
	WebTool::TSqlData sqlData;
	std::string rSql;
	// select * from t_file_pic_info where user_name = User_name;
	// select * from t_file_vid_info where user_id = '001';

	rSql = "select * from t_file_pic_info where user_name = '" + user_name + "'";
	SQL.execSQL(rSql, sqlData);
	std::vector<std::string > Head = sqlData.getHead();
	DBG(L_DEBUG, "��ñ�ͷִ��");

	std::vector<std::string> jsonarray;
	for (int row = 0; row < sqlData.getRow(); row++)
	{
		std::string jsonstr;
		for (int col = 0; col < sqlData.getCol(); col++)
			WebTool::TJson::setStr(jsonstr, Head[col], sqlData.getData(row, col));
		jsonarray.push_back(jsonstr);
	}
	WebTool::TJson::setArray(json, "file_pic_info", jsonarray);

	jsonarray.clear();
	rSql = "select * from t_file_vid_info where user_name = '" + user_name + "'";
	SQL.execSQL(rSql, sqlData);
	Head = sqlData.getHead();
	DBG(L_DEBUG, "��ñ�ͷִ��");
	for (int row = 0; row < sqlData.getRow(); row++)
	{
		std::string jsonstr;
		for (int col = 0; col < sqlData.getCol(); col++)
			WebTool::TJson::setStr(jsonstr, Head[col], sqlData.getData(row, col));
		jsonarray.push_back(jsonstr);

	}
	WebTool::TJson::setArray(json, "file_vid_info", jsonarray);
	return CSQL_SUCCESS;
	// DEbug
	/*for (auto it : jsonarray)
	{
	DBG(L_DEBUG, "%s", WebTool::TJson::getStr(it, "file_size").c_str());
	}
	*/
}

int CSql::UploadFile(const std::string &user_name, const std::string &filejson, const std::string & cla)
{
	// INSERT INTO `t_file_vid_info` (`local_file`, `mongo_file`, `private_attr`, `user_id`, `status`, `user_name`)   \
	VALUES ('llh.mp4', 'llh.mp4', '0', '001', '0', 'wmj')
	std::string local_file = WebTool::TJson::getStr(filejson, "local_file");
	std::string private_attr = WebTool::TJson::getStr(filejson, "private_attr");
	std::string user_id = WebTool::TJson::getStr(filejson, "user_id");

	std::string table;

	if (cla == "vid") table = "t_file_vid_info";
	else if (cla == "pic") table = "t_file_pic_info";
	else return CSQL_E_PARAM;

	std::string rSql = "INSERT INTO `"+ table + "` (`local_file`, `mongo_file`, `private_attr`, `user_id`, `status`, `user_name`)   \
	VALUES ('" + local_file + "', '" + local_file + "', '" + private_attr + "', '" + user_id + "', '0', '" + user_name + "')";

	// test 
	WebTool::TSqlData sqlData;
	if (SQL.execSQL(rSql))
	{
		DBG(L_DEBUG, "�ϴ�ʧ��");
		return CSQL_E_UPDATE;//ʧ�� д��log��
	}
	unsigned long long  file_size;

	//std::string file_path = mongo_tmp_file_path + local_file;
	std::string file_path = local_file;

	if (Mongo.Upload_mongoFile(file_path,local_file,table, file_size) != 0)
	{
		DBG(L_DEBUG, "mongo�ϴ�ʧ��");
		return CSQL_E_UPDATE;
	}
	DBG(L_DEBUG, "mongo�ϴ��ɹ�");
	if (UpdateFileInfo(user_name, local_file, table, file_size) != CSQL_SUCCESS)
	{
		DBG(L_DEBUG, "�����ļ���Ϣʧ��");
		return CSQL_E_UPDATE;
	}
	return CSQL_SUCCESS;
}

int CSql::UpdateFileInfo(const std::string & username, const std::string & file_name, const std::string & table,const unsigned long long & file_size)
{
	char rSql[512] = {0};
	sprintf(rSql, "update %s set file_size=%lld,status='1',remark='upload sucess' \
				 where user_name='%s' and local_file='%s'", table.c_str(), file_size, username.c_str(), file_name.c_str());

	DBG(L_DEBUG,, rSql);
	DBG(L_DEBUG, "����...");
	if (SQL.execSQL(rSql))
	{
		DBG(L_DEBUG, "����ʧ��");
		return CSQL_E_UPDATE;//ʧ�� д��log��
	}
	DBG(L_DEBUG, "���³ɹ�");
	return CSQL_SUCCESS;
}

int CSql::DownloadFile(const std::string & file_name, const std::string & cla, std::string & down_url)
{
	std::string table;
	if (cla == "vid") table = "t_file_vid_info";
	else if (cla == "pic") table = "t_file_pic_info";
	else return CSQL_E_PARAM;

	// UPDATE `t_file_vid_info` SET `download`='1' WHERE (`file_id`='1')
	char rSql[512] = { 0 };
	sprintf(rSql, " UPDATE `%s` SET download=download+1 WHERE (`local_file`='%s')", table.c_str(), file_name.c_str()); 

	// retStr += "X-Accel-Redirect:/png/put.py \r\n";//�¼ӵ� test��

	char uri[512] = { 0 };
	sprintf(uri, "X-Accel-Redirect:/%s/%s \r\n", table.c_str(), file_name.c_str());
	down_url.assign(uri);
	return 0;
}

int CSql::DeleFile(const std::string & User_name, const std::string & file_name, const std::string & cla)
{
	std::string table;
	if (cla == "vid") table = "t_file_vid_info";
	else if (cla == "pic") table = "t_file_pic_info";
	else return CSQL_E_PARAM;
	//mysql��ɾ��
	char rSql[512] = { 0 };
	sprintf(rSql,"delete from %s where local_file = %s and user_name = %s",table.c_str(),file_name.c_str(), User_name.c_str());
	if (SQL.execSQL(rSql))
	{
		DBG(L_DEBUG, "mysqlɾ��ʧ��");
		return CSQL_E_UPDATE;//ʧ�� д��log��
	}
	Mongo.Delete_mongoFile(file_name, table);
	//mongo��ɾ��
	return 0;
}

int CSql::UserLogout(const std::string& user_name)
{
	//������״̬����Ϊ 0 
	// SQL ���  update t_user_info set online='0' where user_name = 'User';
	std::string rSql;
	rSql = "update t_user_info set online='0' where user_name = '" + user_name + "'";//������
	DBG(L_DEBUG, "rSql: %s", rSql.c_str());//������ ��ӡsql���
	if (SQL.execSQL(rSql))
	{
		DBG(L_DEBUG, "����״̬ʧ��");
		return CSQL_E_UPDATE;//ʧ�� д��log��
	}
	DBG(L_DEBUG, "���³ɹ�");
	return CSQL_SUCCESS;//��֤�ɹ�
}

int CSql::Islogin(const std::string & username)
{
	//������֤�Ƿ��¼
	// select online from t_user_info where user_name = 'wmj';
	WebTool::TSqlData sqlData;
	std::string rSql;
	rSql = "select online from t_user_info where user_name = '" + username + "'";
	SQL.execSQL(rSql, sqlData);
	if (sqlData.getRow() > 0 && sqlData.getData(0, 0) != "1") return CSQL_E_LOGIN;
	DBG(L_DEBUG, "��֤��¼�ɹ�");
	return 0;
}

int CSql::InitConnect(const std::string sIp,
	unsigned short uiPort,
	const std::string sUserName,
	const std::string sPassword,
	const std::string sDatabase)
{
	if (SQL.InitTMysql(sIp, uiPort, sUserName, sPassword, sDatabase) != WebTool::MYSQL_SUCCESS)  return CSQL_E_CONNECT;//�����ڲ�Ҫ�ô�����
	if (SQL.openDB() != WebTool::MYSQL_SUCCESS) return CSQL_E_CONNECT;
	// ����mongodb
	if (Mongo.InitTMongo(monmgoHost) != 0) return CSQL_E_CONNECT;
	return CSQL_SUCCESS;
}

int CSql::UserLogin(const std::string& User, const std::string& Passwd)
{
	std::string rSql;
	// �����ж������Ƿ���Ч ��Ч�����������ݿ�
	if (SQL.is_connvalid() != WebTool::MYSQL_SUCCESS)
	{
		DBG(L_DEBUG, "����ʧЧ�����������ӡ���");
		if (SQL.openDB() != WebTool::MYSQL_SUCCESS)
		{
			DBG(L_DEBUG, "����ʧ��");
			return CSQL_E_CONNECT;
		}

	}
	DBG(L_DEBUG, "������Ч");
	//��ֹ sql ע��
	if (Passwd.find("or") != std::string::npos)
	{
		DBG(L_DEBUG, "SQLע��");
		return CSQL_E_CONNECT;
	}
	rSql = "select user_id from t_user_info where user_name = '" + User + "' and pass_word = '" + Passwd + "'";//������
	DBG(L_DEBUG, "rSql: %s", rSql.c_str());//������ ��ӡsql���
	WebTool::TSqlData sqlData;
	if (SQL.execSQL(rSql, sqlData))
	{
		DBG(L_DEBUG, "��ѯʧ��");
		return CSQL_E_QUERRY;//ʧ�� д��log��
	}
	if (sqlData.getRow() == 0)
	{
		DBG(L_DEBUG, "��֤ʧ��");
		return CSQL_E_LOGIN;//��֤ʧ��
	}
	DBG(L_DEBUG, "��֤�ɹ�");
	//������״̬����Ϊ 1 
	// SQL ���  update t_user_info set online='1' where user_name = 'User';
	rSql = "update t_user_info set online='1' where user_name = '" + User + "'";//������
	DBG(L_DEBUG, "rSql: %s", rSql.c_str());//������ ��ӡsql���
	if (SQL.execSQL(rSql))
	{
		DBG(L_DEBUG, "����״̬ʧ��");
		return CSQL_E_UPDATE;//ʧ�� д��log��
	}
	DBG(L_DEBUG, "���³ɹ�");
	return CSQL_SUCCESS;//��֤�ɹ�

}
//test
//#include <TEncode.h>
//#include <TConf.h>
//#include <TFile.h>
//#include <Template.h>
//#include <TString.h>
//using namespace WebTool;
//static TConf gConf("fcgi.conf");
//int main()
//{
//	TString type = gConf.getConfStr("[LOG]", "type", "file");
//	TString fileName = gConf.getConfStr("[LOG]", "fileName", "fcgi.log");
//	TString ip = gConf.getConfStr("[LOG]", "ip", "127.0.0.1");
//	TString port = gConf.getConfStr("[LOG]", "port", "9090");
//	TString threadCount = gConf.getConfStr("[ROUTE]", "threadCount", "10");
//
//	TLOG->initConfig(L_DEBUG, type, fileName, ip, port.toInt());
//
//	CSql sql;
//	std::string json;
//	sql.InitConnect("localhost", 3306, "root", "123456", "web");
//	std::string filejson;
//	WebTool::TJson::setStr(filejson, "local_file", "m.sh");
//	WebTool::TJson::setStr(filejson, "private_attr", "1");
//	WebTool::TJson::setStr(filejson, "user_id", "001");
//	sql.UploadFile("wmj", filejson, "pic");
//
//	sql.ListAllFile(json);
//	std::vector<std::string> jsonarry;
//	jsonarry = WebTool::TJson::getArray(json, "file_pic_info");
//	for (auto testStr : jsonarry)
//	{
//		std::cout << WebTool::TJson::getStr(testStr, "local_file") << std::endl;
//	}
//	jsonarry.clear();
//	jsonarry = WebTool::TJson::getArray(json, "file_vid_info");
//	for (auto testStr : jsonarry)
//	{
//		std::cout << WebTool::TJson::getStr(testStr, "local_file") << std::endl;
//	}
//
//	sql.UpdateFileInfo("wmj", "wmj.py", "pic", 123);
//}
