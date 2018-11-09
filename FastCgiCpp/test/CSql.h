#pragma once
#include"../tools/TSql.h"
#include<string>



class CSql
{
public:
	CSql();
	~CSql();
	int UserLogin(const std::string& User, const std::string& Passwd);// c语言


private:
	std::string U_ID;
	WebTool::TMysql SQL{"218.192.170.66", 3306, "root", "123456", "web"};//在类内部要用大括号
};

