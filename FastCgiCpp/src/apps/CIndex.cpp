#include "CIndex.h"
#include "TJson.h"
#include "Session.h"
#include<map>

///@ /index
Response CIndex::index(Request req)
{
    DBG(L_DEBUG, "method : %s,  url: %s,  params: %s, cookie: %s",
        req.getMethod().c_str(),
        req.getUrl().c_str(),
        req.getParams().c_str(),
        req.getCookie().c_str());

    Params reqParams(req.getParams());
    Cookie reqCookie(req.getCookie());
    Response res;
    TString data;
    if("GET" == req.getMethod())
    {
        if(SESSION_PWD == WebTool::TEncode::base64Decode(reqCookie["session"]))
        {
            //login
            TString dataStr;
            dataStr.loadFile(TString(HTML_PATH) + "index.html");

            WebTool::Template temp(TString(HTML_PATH) + "template.html");
            temp.set("newTitle1", "首页");
            temp.set("newTitle2", "我的桌面");
            temp.set("newHtml", dataStr);
            data = temp.toStr();
        }
        else
        {
            //not login
            data.loadFile(TString(HTML_PATH) + "login.html");
        }
        reqCookie.delCookie("password");
        res.setSetCookie(reqCookie);
        res.setResData(data);

    }
    else if("POST" == req.getMethod())
    {
		DBG(L_DEBUG, "45");
        // 用户登陆验证

		/*********************************************************************/
		/****获取并验证客户端提交的参数***************************************/
		/*********************************************************************/

		std::string username = reqParams["username"];
		std::string password = reqParams["password"];
		TString verify = reqParams["verify"];
		std::string ip = req.getIp();

		/*********************************************************************/
		/****从session中读取验证码信息***************************************/
		/*********************************************************************/

		std::string verify_code = MySession->getVerify();// 获取验证码

		// 删除session中的验证码（验证码每提交一次就失效）;
		MySession->delVerify();

		// 判断用户提交的验证码和存储在session中的验证码是否相同
		verify.toUpper();
		if (verify != verify_code)
		{   
			// 验证码错误
			std::string msg = WebTool::web_helper::return_msg(-1, "验证码错误");
			data = msg;
			res.setResData(data);
			return res;
		}

		/*********************************************************************/
		/****获取登录用户记录，并进行登录验证*********************************/
		/*********************************************************************/


		if (!CSQL->UserLogin(reqParams["username"], reqParams["password"]))
        {
            //login success
			// 把用户信息保存到session中
			std::map<std::string, std::string> info;
			CSQL->returnInfo(info, reqParams["username"]);
			std::string session = MySession->addSessionID(info["id"]);
			MySession->setLoginName(session, info["login_name"]);

			std::string msg = WebTool::web_helper::return_msg(0, "登陆成功");

			data = msg;
            reqCookie.setCookie("sessionId", session);
            res.setSetCookie(reqCookie);
        }
        else
        {
            //login fail
			std::string msg = WebTool::web_helper::return_msg(-1, "密码或用户名错误");
			data = msg;
        }
        res.setResData(data);
    }
    return res;
}
