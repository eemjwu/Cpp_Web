#include "TRegex.h"
#include <map>
using namespace std;

namespace WebTool
{
	
	TRegex::TRegex()
	{
	}


	TRegex::~TRegex()
	{
	}

	bool TRegex::check_string(const string & s, const std::string & pattern)
	{
		regex r(pattern);
		smatch results;
		return regex_search(s, results, r);
	}

	bool TRegex::is_email(const string & s)
	{

		return check_string(s, "[^\\._-][\\w\\.-]+@(?:[A-Za-z0-9]+\\.)+[A-Za-z]+$");//c++ 必须用两个反斜杠
	}

	bool TRegex::is_phone(const string & s)
	{
		return check_string(s, "\\(?0\\d{2,3}[) -]?\\d{7,8}$");
	}

	bool TRegex::is_mobile(const string & s)
	{

		return check_string(s, "^1[3578]\\d{9}$|^147\\d{8}$");
	}

	bool TRegex::is_letters(const string & s)
	{
		return check_string(s, "^[a-zA-Z]+$");
	}

	bool TRegex::is_idcard(TString & s)
	{
		s.toUpper();
		IdentityCard ic;
		return ic.check(s);
		
	}

	std::string TRegex::replaceCharEntity(TString& htmlstr)
	{
		std::string res = htmlstr;
		map<string, string> CHAR_ENTITIES = { { "&nbsp"," " },{ "&160" ," " },
		{ "&lt","<" },{ "&60","<" },
		{ "&gt",">" },{ "&62",">" },
		{ "&amp","&" },{ "&38","&" },
		{ "&quot","\"" },{ "&34","\"" } };
		regex r("(&)#?(\\w+);");
		smatch result;
		while (regex_search(htmlstr, result, r))
		{
			if (CHAR_ENTITIES.find(result.str()) != CHAR_ENTITIES.end())
			{
				htmlstr.replace(result.str(), CHAR_ENTITIES[result.str()]);
			}
			else htmlstr.replace(result.str(), "");

		}
		return htmlstr;

	}
	std::string TRegex::filter_str(const string & s, const string & pattern)
	{
		regex r(pattern);
		string fmt = "";
		if (s != "")
		{
			return regex_replace(s, r, fmt);
		}
		else return "";
	}

	std::string TRegex::filter_tags(string& htmlstr)
	{
		regex re_cdata("//<!\\[CDATA\\[[^>]*//\\]\\]>");
		regex re_script("<\\s*script[^>]*>[^<]*<\\s*/\\s*script\\s*>");
		regex re_style("<\\s*style[^>]*>([^<]*)<\\s*/\\s*style\\s*>");
		regex re_br("<br\\s*?/?>");
		regex re_h("</?\\w+[^>]*>");
		regex re_comment("<!--[^>]*-->");

		TString tmp = regex_replace(htmlstr, re_cdata, "");

		tmp = regex_replace(tmp, re_script, "");
		tmp = regex_replace(tmp, re_style, "");

		tmp = regex_replace(tmp, re_br, "\n");
		tmp = regex_replace(tmp, re_h, "");
		tmp = regex_replace(tmp, re_comment, "");

		regex blank_line("\\n+");
		tmp = regex_replace(tmp, blank_line, "\n");

		tmp = replaceCharEntity(tmp);

		return tmp;
	}




	char IdentityCard::calculate(const TString & code)
	{
		int sum = 0;
		for (int i = 0; i < 17; i++)
		{
			sum += int(code[i] - '0') * Wi[i];
		}
		return Ti[sum%11];
	}

	bool IdentityCard::check(const TString& code)
	{
		if (code.size() != 18) return 0;
		if (calculate(code) != code[17]) return 0;
		return 1;
	}

}

#ifdef TEST_MAIN
#include<exception>
int main(int args, char* argv[])
{
	WebTool::TRegex T;
	string s = "<label for=\"online\"> \
		<input type = \"checkbox\" name = \"online\" id = \"online\" value = \"\"> \
		使我保持登录状态< / label>";
	string res;
	res = T.filter_tags(s);
	cout << res << endl;

}


#endif
