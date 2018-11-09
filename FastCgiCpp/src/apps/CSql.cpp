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
	DBG(L_DEBUG, "获得表头执行");
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
	DBG(L_DEBUG, "获得表头执行");
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
	DBG(L_DEBUG, "进入listinfo");
	//首先验证是否登录
	// select online from t_user_info where user_name = 'wmj';
	WebTool::TSqlData sqlData;
	std::string rSql;
	// select * from t_file_pic_info where user_name = User_name;
	// select * from t_file_vid_info where user_id = '001';

	rSql = "select * from t_file_pic_info where user_name = '" + user_name + "'";
	SQL.execSQL(rSql, sqlData);
	std::vector<std::string > Head = sqlData.getHead();
	DBG(L_DEBUG, "获得表头执行");

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
	DBG(L_DEBUG, "获得表头执行");
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
		DBG(L_DEBUG, "上传失败");
		return CSQL_E_UPDATE;//失败 写入log中
	}
	unsigned long long  file_size;

	//std::string file_path = mongo_tmp_file_path + local_file;
	std::string file_path = local_file;

	if (Mongo.Upload_mongoFile(file_path,local_file,table, file_size) != 0)
	{
		DBG(L_DEBUG, "mongo上传失败");
		return CSQL_E_UPDATE;
	}
	DBG(L_DEBUG, "mongo上传成功");
	if (UpdateFileInfo(user_name, local_file, table, file_size) != CSQL_SUCCESS)
	{
		DBG(L_DEBUG, "更新文件信息失败");
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
	DBG(L_DEBUG, "更新...");
	if (SQL.execSQL(rSql))
	{
		DBG(L_DEBUG, "更新失败");
		return CSQL_E_UPDATE;//失败 写入log中
	}
	DBG(L_DEBUG, "更新成功");
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

	// retStr += "X-Accel-Redirect:/png/put.py \r\n";//新加的 test用

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
	//mysql中删除
	char rSql[512] = { 0 };
	sprintf(rSql,"delete from %s where local_file = %s and user_name = %s",table.c_str(),file_name.c_str(), User_name.c_str());
	if (SQL.execSQL(rSql))
	{
		DBG(L_DEBUG, "mysql删除失败");
		return CSQL_E_UPDATE;//失败 写入log中
	}
	Mongo.Delete_mongoFile(file_name, table);
	//mongo中删除
	return 0;
}

int CSql::UserLogout(const std::string& user_name)
{
	//将在线状态设置为 0 
	// SQL 语句  update t_user_info set online='0' where user_name = 'User';
	std::string rSql;
	rSql = "update t_user_info set online='0' where user_name = '" + user_name + "'";//加引号
	DBG(L_DEBUG, "rSql: %s", rSql.c_str());//调试用 打印sql语句
	if (SQL.execSQL(rSql))
	{
		DBG(L_DEBUG, "更新状态失败");
		return CSQL_E_UPDATE;//失败 写入log中
	}
	DBG(L_DEBUG, "更新成功");
	return CSQL_SUCCESS;//验证成功
}

int CSql::Islogin(const std::string & username)
{
	//首先验证是否登录
	// select online from t_user_info where user_name = 'wmj';
	WebTool::TSqlData sqlData;
	std::string rSql;
	rSql = "select online from t_user_info where user_name = '" + username + "'";
	SQL.execSQL(rSql, sqlData);
	if (sqlData.getRow() > 0 && sqlData.getData(0, 0) != "1") return CSQL_E_LOGIN;
	DBG(L_DEBUG, "验证登录成功");
	return 0;
}

int CSql::InitConnect(const std::string sIp,
	unsigned short uiPort,
	const std::string sUserName,
	const std::string sPassword,
	const std::string sDatabase)
{
	if (SQL.InitTMysql(sIp, uiPort, sUserName, sPassword, sDatabase) != WebTool::MYSQL_SUCCESS)  return CSQL_E_CONNECT;//在类内部要用大括号
	if (SQL.openDB() != WebTool::MYSQL_SUCCESS) return CSQL_E_CONNECT;
	// 连接mongodb
	if (Mongo.InitTMongo(monmgoHost) != 0) return CSQL_E_CONNECT;
	return CSQL_SUCCESS;
}

int CSql::UserLogin(const std::string& User, const std::string& Passwd)
{
	std::string rSql;
	// 首先判断连接是否有效 无效重新连接数据库
	if (SQL.is_connvalid() != WebTool::MYSQL_SUCCESS)
	{
		DBG(L_DEBUG, "连接失效。。重新连接。。");
		if (SQL.openDB() != WebTool::MYSQL_SUCCESS)
		{
			DBG(L_DEBUG, "重连失败");
			return CSQL_E_CONNECT;
		}

	}
	DBG(L_DEBUG, "连接有效");
	//防止 sql 注入
	if (Passwd.find("or") != std::string::npos)
	{
		DBG(L_DEBUG, "SQL注入");
		return CSQL_E_CONNECT;
	}
	rSql = "select user_id from t_user_info where user_name = '" + User + "' and pass_word = '" + Passwd + "'";//加引号
	DBG(L_DEBUG, "rSql: %s", rSql.c_str());//调试用 打印sql语句
	WebTool::TSqlData sqlData;
	if (SQL.execSQL(rSql, sqlData))
	{
		DBG(L_DEBUG, "查询失败");
		return CSQL_E_QUERRY;//失败 写入log中
	}
	if (sqlData.getRow() == 0)
	{
		DBG(L_DEBUG, "验证失败");
		return CSQL_E_LOGIN;//验证失败
	}
	DBG(L_DEBUG, "验证成功");
	//将在线状态设置为 1 
	// SQL 语句  update t_user_info set online='1' where user_name = 'User';
	rSql = "update t_user_info set online='1' where user_name = '" + User + "'";//加引号
	DBG(L_DEBUG, "rSql: %s", rSql.c_str());//调试用 打印sql语句
	if (SQL.execSQL(rSql))
	{
		DBG(L_DEBUG, "更新状态失败");
		return CSQL_E_UPDATE;//失败 写入log中
	}
	DBG(L_DEBUG, "更新成功");
	return CSQL_SUCCESS;//验证成功

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
