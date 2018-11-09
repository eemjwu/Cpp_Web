#include "CSql.h"



CSql::CSql()
{
}


CSql::~CSql()
{
}

int CSql::UserLogin(const std::string& User, const std::string& Passwd)
{
	
	// 首先连接数据库
	if (SQL.openDB()) return  -1;//连接失败
	//防止 sql 注入
	if (Passwd.find("or") != std::string::npos) return -1;//sql 注入 连接失败
	std::string rSql = "select user_id from t_user_info where user_id = " + User + " and pass_word = " + Passwd;
	WebTool::TSqlData sqlData;
	if(SQL.execSQL(rSql, sqlData)) return -1;//失败 应该写入log中
	U_ID = sqlData.getData(0, "user_id");
	return 0;//验证成功

}