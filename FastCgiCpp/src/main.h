/////////////////////////////////////////////
///COPYRIGHT NOTICE
/// Copyright (c) 2018
/// All rights reserved.
///
/// @file main.h
/// @brief һЩȫ�ְ�����ͷ�ļ������ú�
///
/// @version 1.0
/// @author liukang
/// @date 2017.12.15
//////////////////////////////////////////////
#pragma once

#include <iostream>
#include <stdlib.h>
#include <sys/types.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "route/Route.h"
#include "route/Cookie.h"
#include "route/Params.h"
#include "route/Request.h"
#include "route/Session.h"


#include "apps/CIndex.h"
#include "apps/CLogin.h"
#include "apps/CArticle.h"
#include  "apps/CSql.h"
#include "apps/Verify.h"
#include "apps/CAbout.h"
#include "apps/CProduct.h"
#include "apps/CProducts_lists.h"

#include "TEncode.h"
#include "TConf.h"
#include "Template.h"
#include "TFile.h"


static TConf gConf("fcgi.conf");

#define HTML_PATH   "/home/liukang/workspace/FastCgiCpp/html/example/"

#define SESSION_PWD "abcdefgliukanghahatest"
#define TEST_USER   "liukang"
#define TEST_PWD    "325"
