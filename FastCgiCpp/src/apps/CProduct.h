#pragma once
#include "../main.h"
class CProduct
{
public:
	CProduct() = default;

	~CProduct() = default;

	Response product(Request req);

private:
	//要用到TMysql里面的成员函数，定义一个对象
	WebTool::TMysql SQL;
};

