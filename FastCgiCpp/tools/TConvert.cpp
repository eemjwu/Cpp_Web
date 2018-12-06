#include "TConvert.h"
using namespace std;
namespace WebTool
{
	int TConvert::to_int(const std::string & s)
	{
		try
		{
			int ret = atoi(s.c_str());
			return ret;
		}
		catch(...)
		{
			return 0;
		}

	}

	int TConvert::to_int0(const std::string & s)
	{
		int res = to_int(s);
		// 判断转换后的结果值是否小于0，是的话返回0
		if (res < 0) return 0;
		else return res;
	}

	int TConvert::to_int1(const std::string & s)
	{
		int res = to_int(s);
		if (res < 1) return 1;
		else return res;
	}

	float TConvert::to_float(const std::string & s)
	{
		try 
		{
			float res = atof(s.c_str());
			return res;
		}
		catch (std::exception e)
		{
			return 0.0;
		}
	
	}

	TConvert::TConvert()
	{
	}

	TConvert::~TConvert()
	{
	}
	std::string TConvert::to_number()
	{

		std::string res;
		time_t rawtime;
		struct tm *info;
		time(&rawtime);
		info = localtime(&rawtime);
		char buffer[80];
		strftime(buffer, 80, "%Y%m%d%H%M%S", info);
		res = buffer;
		return res;
	}

	int TConvert::to_timestamp10()
	{
		time_t rawtime;
		time(&rawtime);
		return rawtime;
	}
	long TConvert::to_timestamp13()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec * 1000 + tv.tv_usec / 1000;

	}
	std::string TConvert::date_to_string(const TDate & dt)
	{
		std::string res;
		res = dt.mystrftime("%Y-%m-%d");
		return res;
	}
	TDate TConvert::to_date(TString& s)
	{
		TDate dt;
		vector<TString> res_s = s.split("-");
		dt.setDate(atoi(res_s[0].c_str()), atoi(res_s[1].c_str()), atoi(res_s[2].c_str()));
		return dt;
	}
	TDate TConvert::timedelta(const std::string sign, const TDate & dt, int value)
	{
		TDate res_dt;
		if (sign == "y")
		{
			int year = dt.getYear() + value;
			res_dt.setDate(year, dt.getMonth(), dt.getDay());
			return res_dt;
		}
		else if (sign == "m")
		{
			int year = dt.getYear();
			int month = dt.getMonth() + value;
			// 如果月份加减后超出范围，则需要计算一下，对年份进行处理
			if (month == 0)
			{
				year = year - 1;
				month = 12;
			}
			else
			{
				year = year + month / 12;
				month = month % 12;
			}
			res_dt.setDate(year, month, dt.getDay());
			return res_dt;
		}
		else if (sign == "w")
		{
			int day = 7 * value;
			res_dt = dt + day;
			return res_dt;
		}
		else if (sign == "d")
		{

			int day = value;
			res_dt = dt + day;
			return res_dt;
		}
		else
		{
			// sign 格式错误
			// 抛出异常
			return res_dt;
		}
		// TODO: 在此处插入 return 语句
	}
}

#ifdef TEST_MAIN
int main()
{
	using namespace std;
	WebTool::TConvert T;
	/*int i_r;
	i_r = T.to_float("1");
	cout << i_r << endl;
	i_r = T.to_float("1.2");
	cout << i_r << endl;
	i_r = T.to_float("la");
	cout << i_r << endl;
	i_r = T.to_float("aa");
	cout << i_r << endl;
	i_r = T.to_float(" ");
	cout << i_r << endl;
	i_r = T.to_float("-1");
	cout << i_r << endl;
	i_r = T.to_float("-1.2");
	cout << i_r << endl;

	string s;
	s = T.to_number();
	cout << s << endl;

	i_r = T.to_timestamp10();
	cout << i_r << endl;

	long l = T.to_timestamp13();
	cout << l << endl;

	WebTool::TDate dt;

	dt.Today();

	s = T.to_date(dt);
	cout << s << endl;

	WebTool::TDate dt1;

	dt1 = T.timedelta("d", dt, 2);

	dt1.show();*/
	WebTool::TString s = "2018-11-11";
	WebTool::TDate dt =  T.to_date(s);
	dt.show();
}
#endif

