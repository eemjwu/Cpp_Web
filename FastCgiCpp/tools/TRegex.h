#pragma once
#include"TString.h"
#include<regex>
#include<string>
#include<iostream>
#include<vector>
namespace WebTool
{
	class TRegex
	{
	public:
		TRegex();
		~TRegex();

		///@brief ����ַ����Ƿ����ָ���Ĺ���
		///@param s:��Ҫ�����ַ���
		///@param pattern:������ʽ
		///@return ����ָ���ַ�ʱ�����棬����Ϊ��
		bool check_string(const std::string& s, const std::string& pattern);

		bool is_email(const std::string& s);

		bool is_phone(const std::string& s);

		bool is_mobile(const std::string& s);

		bool is_letters(const std::string& s);

		bool is_idcard(TString& s);

		std::string replaceCharEntity(TString& htmlstr);

		std::string filter_str(const std::string & s, const std::string & pattern = "\\||<|>|&|%|~|\\^|;|\\'");

		std::string filter_tags(std::string& htmlstr);

		
	};
	class IdentityCard
	{
	public:
		IdentityCard() {};
		~IdentityCard() {};
	
		bool check(const TString& code);
	private:
		std::vector<int> Wi{ 7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2 };
		std::vector<char> Ti{ '1','0','X','9','8','7','6','5','4','3','2' };
		char calculate(const TString& code);
	};


}
