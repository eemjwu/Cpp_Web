/////////////////////////////////////////////
///COPYRIGHT NOTICE
/// Copyright (c) 2018
/// All rights reserved.
///
/// @file CArticle.h
/// @brief
///
/// @version 1.0
/// @author liukang
/// @date 2018.01.28
//////////////////////////////////////////////
#pragma once

#include "../main.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fcgiapp.h>
#include <string.h>
#include <fcgi_stdio.h>

class  CArticle
{
public:
    CArticle() = default;
    ~CArticle() = default;

    Response articleList(Request req);


private:

};
