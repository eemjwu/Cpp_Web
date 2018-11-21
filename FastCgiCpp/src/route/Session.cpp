#include "Session.h"

using namespace std;

int Session::InitSession(int cookie_expires, const std::string& ip, unsigned short p)
{
	expire_time = cookie_expires;//默认3600秒  1小时
	redis_ip = ip;
	redis_port = p;
	if (myredis.InitTRedis(redis_ip, redis_port) == 0)
	{
		DBG(L_DEBUG, "session 初始化成功");
		return 0;
	}
	return -1;
	//应该要抛出异常
	// throw

}

bool Session::existSessionId(const std::string & session_id)
{
	if (myredis.existesKey(session_id.c_str()) == 0)
		return 1;
	return false;
}

std::string Session::getSession(const std::string & session_id)
{
	
	string jsonRes;
	unordered_map<string, string> tmp;
	
	if (myredis.Hget(tmp, session_id.c_str(),";") != 0)
	{
		return "";
	}
	for (auto it : tmp)
	{
		WebTool::TJson::setStr(jsonRes, it.first, it.second);
	}
	return jsonRes;
}

std::string Session::addSessionID(const std::string& conversasionID)
{
	string sessionId;
	random_helper tmp;
	sessionId = static_cast<string>(tmp.get_string(4)) + conversasionID;//产生唯一的sessionId
	if (myredis.Hset(sessionId.c_str(), "login_name", "$", "verify_code", "$", ";") != 0) return "";
	if (myredis.expiresSet(sessionId.c_str(), this->expire_time) != 0) return "";
	return sessionId;
}

bool Session::setVerify(const std::string & session_id, WebTool::TString & verify)
{
	verify.toUpper();
	if (myredis.Hset(session_id.c_str(), "verify_code", verify.c_str(), ";") != 0) return false;
	return true;
}

bool Session::setLoginName(const std::string & session_id, const std::string & login_name)
{
	if (myredis.Hset(session_id.c_str(), "login_name", login_name.c_str(), ";") != 0) return false;
	return true;
}

#ifdef TEST_MAIN

int main()
{
	TString x = "1";

	Session session;
	string ID = session.addSessionID("1");
	cout << session.existSessionId(ID) << endl;
	string sessionJson = session.getSession(ID);

	string verify = TJson::getStr(sessionJson, "verify_code");
	string login_name = TJson::getStr(sessionJson, "login_name");

	cout << "verify:" << verify << endl;
	cout << "login_name:" << login_name << endl;

	WebTool::TString v = "ABCD";
	session.setVerify(ID, v);
	session.setLoginName(ID, "wmj");

	sessionJson = session.getSession(ID);
	verify = WebTool::TJson::getStr(sessionJson, "verify_code");
	login_name = WebTool::TJson::getStr(sessionJson, "login_name");
	cout << "verify:" << verify << endl;
	cout << "login_name:" << login_name << endl;
}

#endif
