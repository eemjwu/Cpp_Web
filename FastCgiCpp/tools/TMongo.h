/////////////////////////////////////////////
///COPYRIGHT NOTICE
/// Copyright (c) 2018
/// All rights reserved.
///
/// @file TMongo.h
/// @brief 封装易用的MongoDB数据库处理类
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

		///@brief   初始化类
		///@return  错误码
		int InitTMongo(const std::string Host);

		///@brief	上传文件
		///@param	文件名
		///@param	文件类型 table
		///@param	返回 文件大小
		///@return	错误码
		int Upload_mongoFile(const std::string & File_path, const std::string & File_name,const std::string & table, unsigned long long & size);

		///@brief	删除文件
		///@param	文件名
		///@param	文件类型 table
		///@return	错误码
		int Delete_mongoFile(const std::string & File_name, const std::string & table);

	private:
		
	};
}


