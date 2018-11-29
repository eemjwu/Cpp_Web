/////////////////////////////////////////////
///COPYRIGHT NOTICE
/// Copyright (c) 2018
/// All rights reserved.
///
/// @file Request.h
/// @brief 封装 Request/Response 类
///
/// @version 1.0
/// @author liukang
/// @date 2017.12.15
//////////////////////////////////////////////
#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <functional>
#include <fcgi_stdio.h>
#include "fcgi_config.h"
#include "TLog.h"
#include "TString.h"
#include "Cookie.h"

using namespace WebTool;

class  Request
{
public:
    Request();
    ~Request();

    void setMethod(TString str);
    void setUrl(TString str);
    void setParams(TString str);
    void setCookie(TString str);
	void setIp(TString str);


    TString getMethod();
    TString getUrl();
    TString getParams();
    TString getCookie();
	TString getIp();

private:

    TString m_method;
    TString m_url;
    TString m_params;
    TString m_cookie;
	TString m_ip;


};


class  Response
{
public:
    Response();
    ~Response();

    void setContentType(const TString str);
    void setSetCookie(const Cookie str);

    void setResData(TString str);

	void setByteData(char data[]);

    TString Out();

	char m_byteDate[17646];
private:

    //Content-type:
    TString m_contentType;

    //Set-Cookie:
    Cookie m_setCookie;

    //html txt
    TString m_resData;

	// 二进制数据流


};
