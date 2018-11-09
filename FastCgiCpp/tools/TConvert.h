/////////////////////////////////////////////
///COPYRIGHT NOTICE
/// Copyright (c) 2018
/// All rights reserved.
///
/// @file TConvert.h
/// @brief 类型转换工具
///
/// 
///
/// @version 1.0
/// @author WuMengJie
/// @date 2018.11.8
//////////////////////////////////////////////
#pragma once
#include<string>
#include <sstream>
#include<ctime>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "TDateTime.h"

namespace WebTool
{
	class TConvert
	{
	public:

		TConvert();
		~TConvert();

		/// @brief  将字符串安全转换为int类型，转换失败时默认值为0
		/// @param  string &
		/// @return int
		int to_int(const std::string &s);

		/// @brief  将字符串安全转换为int类型，当int值小于0时，返回0
		/// @param  string &
		/// @return int
		int to_int0(const std::string &s);

		/// @brief  将字符串安全转换为int类型，当int值小于1时，返回1
		/// @param  string &
		/// @return int
		int to_int1(const std::string &s);

		/// @brief  将字符串安全转换为float类型
		/// @param  string &
		/// @return int
		float to_float(const std::string &s);


		std::string to_number(); //将当前时间转换为年月日时分秒毫秒共10位数的字符串
		int to_timestamp10();//获取当前时间长度为10位长度的时间戳
		long to_timestamp13();//获取当前时间长度为13位长度的时间戳

		std::string to_date(const TDate& date); //将时间格式化为日期字符串

		///对指定时间进行加减运算，几秒、几分、几小时、几日、几周、几月、几年
		///sign : y = 年, m = 月, w = 周, d = 日, h = 时, n = 分钟, s = 秒
		///dt : 日期，只能是datetime或datetime.date类型
		///value : 加减的数值
		///return : 返回运算后的datetime类型值
		TDate timedelta(const std::string sign, const TDate& dt, int value);
			
	};
	


}





