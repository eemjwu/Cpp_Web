/////////////////////////////////////////////
///COPYRIGHT NOTICE
/// Copyright (c) 2018
/// All rights reserved.
///
/// @file CAbout.h
/// @brief 初始页面
///
/// @version 1.0
/// @author 金牌讲师
/// @date 2018.11.27
//////////////////////////////////////////////
#pragma once

#include "../main.h"
#include <stdlib.h> //新增Include
#include <stdio.h>
#include<TSql.h>
#include<string>
#include<TJson.h>

//错误码
//enum
//{
//	CSQL_SUCCESS = 0,//成功
//	CSQL_E_CONNECT = 1,//连接初始化失败
//	CSQL_E_LOGIN = 2,//验证失败
//	CSQL_E_QUERRY = 3, //查询失败
//	CSQL_E_UPDATE = 4, //更新失败
//	CSQL_E_PARAM = 5  //参数错误
//};

class CAbout
{
public:
	CAbout() = default;
	~CAbout() = default;

	Response about(Request req);


private:
	//要用到TMysql里面的成员函数，定义一个对象
	WebTool::TMysql SQL;
};


