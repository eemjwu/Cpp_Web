#include "Request.h"

Request::Request():
    m_method(""),
    m_url(""),
    m_params(""),
    m_cookie(""),
	m_ip("")
{

}

Request::~Request()
{

}

void Request::setMethod(TString str)
{
    m_method = str;
}

void Request::setUrl(TString str)
{
    m_url = str;
}

void Request::setParams(TString str)
{
    m_params = str;
}

void Request::setCookie(TString str)
{
    m_cookie = str;
}

void Request::setIp(TString str)
{
	m_ip = str;
}

void Request::setReqStream(FCGX_Request * req)
{
	request = req;
}



TString Request::getMethod()
{
    return m_method;
}

TString Request::getUrl()
{
    return m_url;
}

TString Request::getParams()
{
    return m_params;
}

TString Request::getCookie()
{
    return m_cookie;
}

TString Request::getIp()
{
	return m_ip;
}

FCGX_Request * Request::getReqStream()
{

	return request;
}

//////////////////////////////////////////////////////
Response::Response():
    m_contentType("text/html"),
    m_setCookie(""),
    m_resData("")
{

}

Response::~Response()
{

}

void Response::setByteData(char data[])
{
	memcpy(m_byteDate,data , sizeof(char) * 17646);
}

void Response::setContentType(const TString str)
{
    m_contentType = str;
}

void Response::setSetCookie(const Cookie cookie)
{
    m_setCookie = cookie;
}

void Response::setResData(TString str)
{
    m_resData = str;
}

TString Response::Out()
{
    TString strSetCookie = m_setCookie.toSetCookieStr();

    TString retStr;
    retStr += "Content-type: " + m_contentType + "\r\n";
    retStr += strSetCookie;
    retStr += "\r\n";

    //DBG(L_DEBUG, "%s", retStr.c_str());
	if (m_contentType != "image/jpg")
	{
		retStr += m_resData;
	}

    return retStr;

}
