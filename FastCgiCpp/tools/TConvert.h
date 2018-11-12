/////////////////////////////////////////////
///COPYRIGHT NOTICE
/// Copyright (c) 2018
/// All rights reserved.
///
/// @file TConvert.h
/// @brief ����ת������
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
#include "TString.h"

namespace WebTool
{
	class TConvert
	{
	public:

		TConvert();
		~TConvert();

		/// @brief  ���ַ�����ȫת��Ϊint���ͣ�ת��ʧ��ʱĬ��ֵΪ0
		/// @param  string &
		/// @return int
		int to_int(const std::string &s);

		/// @brief  ���ַ�����ȫת��Ϊint���ͣ���intֵС��0ʱ������0
		/// @param  string &
		/// @return int
		int to_int0(const std::string &s);

		/// @brief  ���ַ�����ȫת��Ϊint���ͣ���intֵС��1ʱ������1
		/// @param  string &
		/// @return int
		int to_int1(const std::string &s);

		/// @brief  ���ַ�����ȫת��Ϊfloat����
		/// @param  string &
		/// @return int
		float to_float(const std::string &s);


		std::string to_number(); //����ǰʱ��ת��Ϊ������ʱ������빲10λ�����ַ���
		int to_timestamp10();//��ȡ��ǰʱ�䳤��Ϊ10λ���ȵ�ʱ���
		long to_timestamp13();//��ȡ��ǰʱ�䳤��Ϊ13λ���ȵ�ʱ���

		std::string date_to_string(const TDate& date); //��ʱ���ʽ��Ϊ�����ַ���

		TDate to_date(TString&);

		///��ָ��ʱ����мӼ����㣬���롢���֡���Сʱ�����ա����ܡ����¡�����
		///sign : y = ��, m = ��, w = ��, d = ��, h = ʱ, n = ����, s = ��
		///dt : ���ڣ�ֻ����datetime��datetime.date����
		///value : �Ӽ�����ֵ
		///return : ����������datetime����ֵ
		TDate timedelta(const std::string sign, const TDate& dt, int value);
			
	};
	


}





