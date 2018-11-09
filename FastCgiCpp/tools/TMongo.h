/////////////////////////////////////////////
///COPYRIGHT NOTICE
/// Copyright (c) 2018
/// All rights reserved.
///
/// @file TMongo.h
/// @brief ��װ���õ�MongoDB���ݿ⴦����
///
///
/// @version 1.0
/// @author wumenjie
/// @date 2018.10.31
//////////////////////////////////////////////
#pragma once
#include "TLog.h"


namespace WebTool
{
	class TMongo
	{
	public:
		TMongo();
		~TMongo();
		TMongo(const TMongo &) = delete;
		TMongo &operator=(const TMongo &) = delete;

		///@brief   ��ʼ����
		///@return  ������
		int InitTMongo(const std::string Host);

		///@brief	�ϴ��ļ�
		///@param	�ļ���
		///@param	�ļ����� table
		///@param	���� �ļ���С
		///@return	������
		int Upload_mongoFile(const std::string & File_path, const std::string & File_name,const std::string & table, unsigned long long & size);

		///@brief	ɾ���ļ�
		///@param	�ļ���
		///@param	�ļ����� table
		///@return	������
		int Delete_mongoFile(const std::string & File_name, const std::string & table);

	private:
		
	};
}


