#pragma once
#include "../main.h"
#include <stdlib.h> //ÐÂÔöInclude
#include <stdio.h>
#include "string.h"
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "libcaptcha.h"

class Verify
{
public:
	Verify() = default;
	~Verify() = default;

	Response get_verify(Request req);
};

