/////////////////////////////////////////////
///COPYRIGHT NOTICE
/// Copyright (c) 2018
/// All rights reserved.
///
/// @file CAbout.h
/// @brief ��ʼҳ��
///
/// @version 1.0
/// @author ���ƽ�ʦ
/// @date 2018.11.27
//////////////////////////////////////////////
#pragma once

#include "../main.h"
#include <stdlib.h> //����Include
#include <stdio.h>
#include<TSql.h>
#include<string>
#include<TJson.h>

//������
//enum
//{
//	CSQL_SUCCESS = 0,//�ɹ�
//	CSQL_E_CONNECT = 1,//���ӳ�ʼ��ʧ��
//	CSQL_E_LOGIN = 2,//��֤ʧ��
//	CSQL_E_QUERRY = 3, //��ѯʧ��
//	CSQL_E_UPDATE = 4, //����ʧ��
//	CSQL_E_PARAM = 5  //��������
//};

class CAbout
{
public:
	CAbout() = default;
	~CAbout() = default;

	Response about(Request req);


private:
	//Ҫ�õ�TMysql����ĳ�Ա����������һ������
	WebTool::TMysql SQL;
};


