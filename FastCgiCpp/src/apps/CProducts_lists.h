#pragma once
#include "../main.h"
class CProducts_lists
{
public:
	CProducts_lists() = default;
	~CProducts_lists() = default;

	Response products_lists(Request req);
};

