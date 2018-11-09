#include "TDateTime.h"

namespace WebTool {

TDateTime::TDateTime()
{

}

TDateTime::~TDateTime()
{

}

TDate & TDate::Today()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buf[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	year = 1900 + timeinfo->tm_year;
	month = 1 + timeinfo->tm_mon;
	day = timeinfo->tm_mday;
	return *this;
	// TODO: 在此处插入 return 语句
}

void TDate::setDate(int year, int month, int day)
{
	this->year = year;
	this->month = month;
	this->day = day;
}

bool TDate::isLeapYear(int y) const
{
	return y % 4 == 0 && y % 100 != 0 || y % 400 == 0;
}

bool TDate::isLeapYear() const
{
	return year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
}

int TDate::dayofMonth(int year, int month) const
{
	int days = 0;

	switch (month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		days = 31;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		days = 30;
		break;
	case 2:
		days = 28 + isLeapYear(year);
		break;
	}

	return days;
}

void TDate::show() const
{
	std::cout << year << "-" << month << "-" << day << std::endl;
}

std::string TDate::strftime(const char * format) const
{
	std::string res;
	char buffer[80];
	struct tm * timeinfo;
	time_t rawtime;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	timeinfo->tm_year = year - 1900;
	timeinfo->tm_mon = month - 1;
	timeinfo->tm_mday = day;
	//mktime(timeinfo);
	std::strftime(buffer, 80, format, timeinfo);
	res = buffer;
	return res;
}

TDate TDate::changeDays(const int days) const
{
	int yearTemp = year;
	int monthTemp = month;
	int dayTemp = day;

	if (days>0) {
		dayTemp += days;

		while (dayTemp>dayofMonth(yearTemp, monthTemp)) {
			dayTemp -= dayofMonth(yearTemp, monthTemp);

			monthTemp++;
			if (monthTemp>12) {
				yearTemp++;
				monthTemp = 1;
			}
		}
	}
	else {   //days为负数
		dayTemp += days;

		while (dayTemp<1) {
			monthTemp--;
			if (monthTemp<1) {
				yearTemp--;
				monthTemp = 12;
			}
			dayTemp += dayofMonth(yearTemp, monthTemp);
		}
	}

	return TDate(yearTemp, monthTemp, dayTemp);
}

int TDate::distance(const TDate & d) const
{
	//存储平年中某个月1月之前有多少天
	const int DAYS_OF_MONTH[] =
	{ 0,31,59,90,120,151,181,212,243,273,304,334,365 };

	int years = year - d.year;
	int months = DAYS_OF_MONTH[month] - DAYS_OF_MONTH[d.month];
	int days = day - d.day;

	//4年一闰，100的倍数免润，400的倍数再闰
	int totalDays = years * 365 + years / 4 - years / 100 + years / 400
		+ months + days;

	return totalDays;
}

TDate & TDate::operator+=(int days)
{
	if (days == 0)
		return *this;
	else {
		*this = this->changeDays(days);
		return *this;
	}

	// TODO: 在此处插入 return 语句
}

TDate & TDate::operator++()
{
	*this = this->changeDays(1);
	return *this;
	// TODO: 在此处插入 return 语句
}

TDate TDate::operator++(int)
{
	TDate dTemp(*this);
	++(*this);
	return dTemp;
}

TDate & TDate::operator-=(int days)
{
	if (days == 0)
		return *this;
	else {
		*this = this->changeDays(-days);
		return *this;
	}
	// TODO: 在此处插入 return 语句
}

TDate & TDate::operator--()
{
	*this = this->changeDays(-1);
	return *this;
	// TODO: 在此处插入 return 语句
}

TDate TDate::operator--(int)
{
	TDate dTemp(*this);
	--(*this);
	return dTemp;
}

TDate operator +(const TDate &d, const int days)
{
	if (days == 0) {   //如果天数为0，返回当个月
		return d;
	}
	else
		return d.changeDays(days);
}
TDate operator +(const int days, const TDate &d)
{
	if (days == 0) {   //如果天数为0，返回当个月
		return d;
	}
	else
		return d.changeDays(days);
}

TDate operator-(const TDate & d, const int days)
{
	if (days == 0) {   //如果天数为0，返回当个月
		return d;
	}
	else
		return d.changeDays(-days);
}

int operator-(const TDate & d1, const TDate & d2)
{
	if (d1<d2) {
		std::cout << "被减数日期必须大于减数日期！" << std::endl;
		return -1;
	}
	else if (d1 == d2)
		return 0;
	else
		return d1.distance(d2);
}

bool operator>(const TDate & d1, const TDate & d2)
{

	return d1.distance(d2)>0 ? true : false;
}

bool operator>=(const TDate & d1, const TDate & d2)
{
	return d1.distance(d2) >= 0 ? true : false;
}

bool operator<(const TDate & d1, const TDate & d2)
{

	return d1.distance(d2)<0 ? true : false;
}

bool operator<=(const TDate & d1, const TDate & d2)
{

	return d1.distance(d2) <= 0 ? true : false;
}

bool operator==(const TDate & d1, const TDate & d2)
{
	return d1.distance(d2) == 0 ? true : false;

}

bool operator!=(const TDate & d1, const TDate & d2)
{

	return d1.distance(d2) != 0 ? true : false;
}

std::ostream & operator<<(std::ostream & out, const TDate & d)
{
	out << d.getYear() << "-"
		<< d.getMonth() << "-"
		<< d.getDay() << std::endl;
	return out;
	// TODO: 在此处插入 return 语句
}

std::istream & operator>>(std::istream & in, TDate & d)
{
	int year, month, day;

	std::cout << "Input year-month-day:" << std::endl;
	in >> year >> month >> day;

	d.setDate(year, month, day);

	return in;
	// TODO: 在此处插入 return 语句
}

} //namespace WebTool

// TEST
//int main(int args, char* argv[])
//{
//	using namespace std;
//	WebTool::TDate dt;
//
//	dt.Today();
//	int year = dt.getYear();
//	int month = dt.getMonth();
//	int day = dt.getDay();
//	std::cout << year << "-" << month << "-" << day<<std::endl;
//	dt.setDate(2011, 1, 1);
//	dt.show();
//	dt.Today();
//	std::string res;
//	res = dt.strftime("%Y-%b-%d %H:%M:%S");
//	cout << res << endl;
//
//	WebTool::TDate dt1;
//	dt1.setDate(2018, 10, 1);
//	day = dt.distance(dt1);
//	cout << day << endl;
//	dt = 6 + dt;
//	dt.show();
//	dt += 10;
//	dt.show();
//	dt --;
//	dt.show();
//
//
//    return 0;
//}

