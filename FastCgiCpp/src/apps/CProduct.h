#pragma once
#include "../main.h"
class CProduct
{
public:
	CProduct() = default;

	~CProduct() = default;

	Response product(Request req);

private:
	//Ҫ�õ�TMysql����ĳ�Ա����������һ������
	WebTool::TMysql SQL;
};

