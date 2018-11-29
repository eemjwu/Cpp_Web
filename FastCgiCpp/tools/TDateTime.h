
///COPYRIGHT NOTICE
/// Copyright (c) 2018
/// All rights reserved.
///
/// @file TDateTime.h
/// @brief 时间格式的封装
///
/// GMT格式等，时间比较操作
///
/// @version 1.0
/// @author WuMengJie
/// @date 2018.11.08
//////////////////////////////////////////////
#pragma once

#include <string>
#include<ctime>
#include<iostream>

namespace WebTool {



class TDate
{
public:
	//默认构造函数
	TDate():year(1900),month(1),day(1){}
	// 对日期赋值 ，判断日期有效性
	TDate(int y, int m, int d) :year(y), month(m), day(d)
	{
		if ((y <= 0) || (m <= 0 || m > 12) || (d <= 0 || d > dayofMonth(y, m)))
		{
			// 给的值无效
			year = 1900;
			month = day = 1;
		}
	}
	 ~TDate(){}
	//拷贝构造函数
	TDate(const TDate &d) :year(d.year), month(d.month), day(d.day) {}
	//拷贝赋值运算
	TDate & operator=(const TDate& d) {
	
		year = d.year;
		month = d.month;
		day = d.day;
		return *this;
	}
	/// @brief  得到当前的日期
	/// @return TDate类
	TDate & Today();

	std::string now();//获取当前时间日期  以字符串返回

	//得到年与日
	int getYear()const { return year; }
	int getMonth()const { return month; }
	int getDay()const { return day; }

	void setDate(int year, int month, int day);//设置日期
	bool isLeapYear(int y)const;   //判断是否为闰年
	bool isLeapYear()const;   //判断是否为闰年
	int dayofMonth(int year, int month)const;   //得到某个月的天数
	void show()const;   //显示日期
	std::string mystrftime(const char *format) const;//日期转为字符串
	TDate changeDays(const int days)const;   //改变 当前日期的  前后几天

	int distance(const TDate &d)const;   //计算两个日期之间的天数

	/*重载运算符*/

										 //日期加上days个天数
	friend TDate operator +(const TDate &d, const int days);
	friend TDate operator +(const int days, const TDate &d);
	TDate& operator +=(int days);
	//日期自增一天
	TDate& operator ++();
	TDate operator ++(int);

	//日期减去days个天数
	friend TDate operator -(const TDate &d, const int days);
	friend int operator -(const TDate &d1, const TDate &d2);
	TDate& operator -=(int days);
	//日期自减一天
	TDate& operator --();
	TDate operator --(int);

	//日期大小比较
	friend bool operator >(const TDate &d1, const TDate &d2);
	friend bool operator >=(const TDate &d1, const TDate &d2);
	friend bool operator <(const TDate &d1, const TDate &d2);
	friend bool operator <=(const TDate &d1, const TDate &d2);
	friend bool operator ==(const TDate &d1, const TDate &d2);
	friend bool operator !=(const TDate &d1, const TDate &d2);

	//输出，输入日期
	friend std::ostream& operator <<(std::ostream &out, const TDate &d);
	friend std::istream& operator >>(std::istream &in, TDate &d);

private:
	int year, month, day;
};


} //namespace WebTool
