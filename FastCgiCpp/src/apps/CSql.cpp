#include "CSql.h"


CSql::CSql()
{
}


CSql::~CSql()
{
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
	//if (Mongo.InitTMongo(monmgoHost) != 0) return CSQL_E_CONNECT;
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
	rSql = "select login_name , is_enable from manager where login_name = '" + User + "' and login_password = '" + Passwd + "'";//������
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
	if (sqlData.getData(0, "is_enable") != "1")
	{
		DBG(L_DEBUG, "û��Ȩ��");
		return CSQL_E_LOGIN;//��֤ʧ��
	}
	DBG(L_DEBUG, "��֤�ɹ�");
	//������״̬����Ϊ 1 
	// SQL ���  update t_user_info set online='1' where user_name = 'User';

	
	return CSQL_SUCCESS;//��֤�ɹ�

}
int CSql::UpdateLoginIp(const std::string & User, const std::string & Ip)
{
	char rSql[512] = { 0 };
	WebTool::TDate datetime;
	std::string t = datetime.now();

	sprintf(rSql, "update manager set last_login_time='%s', last_login_ip='%s', login_count=login_count+1 where login_name='%s'" \
		, t.c_str(), Ip.c_str(),User.c_str());

	DBG(L_DEBUG, "rSql: %s", rSql);//������ ��ӡsql���
	if (SQL.execSQL(static_cast<std::string>(rSql)))
	{
		DBG(L_DEBUG, "����״̬ʧ��");
		return CSQL_E_UPDATE;//ʧ�� д��log��
	}
	DBG(L_DEBUG, "���³ɹ�");
}
int CSql::returnInfo(std::map<std::string,std::string> res,const std::string & User)
{
	char rSql[512] = { 0 };
	sprintf(rSql, "select * from manager where login_name = '%s'",User.c_str());
	DBG(L_DEBUG, "rSql: %s", rSql);//������ ��ӡsql���
	WebTool::TSqlData sqlData;
	if (SQL.execSQL(static_cast<std::string>(rSql), sqlData))
	{
		DBG(L_DEBUG, "��ѯʧ��");
		return CSQL_E_UPDATE;//ʧ�� д��log��
	}
	if (sqlData.getRow() == 0)
	{
		DBG(L_DEBUG, "��ѯʧ��");
		return CSQL_E_LOGIN;//��֤ʧ��
	}
	res["id"] = sqlData.getData(0, "id");
	res["login_name"] = sqlData.getData(0, "login_name");
	DBG(L_DEBUG, "��֤�ɹ�");
	return 0;
}
int CSql::execSQL(const std::string sSqlStr)
{
	return SQL.execSQL(sSqlStr);
	
}
int CSql::execSQL(const std::string sSqlStr, WebTool::TSqlData & sqlData)
{
	return SQL.execSQL(sSqlStr, sqlData);
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
