#include "Session.h"

using namespace std;

int Session::InitSession(int cookie_expires, const std::string& ip, unsigned short p)
{
	expire_time = cookie_expires;//Ĭ��3600��  1Сʱ
	redis_ip = ip;
	redis_port = p;
	if (myredis.InitTRedis(redis_ip, redis_port) == 0)
	{
		DBG(L_DEBUG, "session ��ʼ���ɹ�");
		return 0;
	}
	return -1;
	//Ӧ��Ҫ�׳��쳣
	// throw

}

bool Session::existSessionId(const std::string & session_id)
{
	if (myredis.existesKey(session_id.c_str()) == 0)
		return 0;
	return -1;
}

std::string Session::getSession(const std::string & session_id)
{
	
	string jsonRes;
	unordered_map<string, string> tmp;

	update_expire_time(session_id);// ÿ��ѯһ�� ����ʱ��
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
	sessionId = static_cast<string>(tmp.get_string(4)) + conversasionID;//����Ψһ��sessionId
	if (myredis.Hset(sessionId.c_str(), "login_name", "$", ";") != 0) return "";
	if (myredis.expiresSet(sessionId.c_str(), this->expire_time) != 0) return "";
	return sessionId;
}

bool Session::setVerify(WebTool::TString & verify)
{
	verify.toUpper();
	if (myredis.Set("verify_code", verify.c_str()) != 0)return false;
	return true;
}

std::string Session::getVerify()
{
	std::string res;
	if (myredis.Get(res, "verify_code") == 0)
	{
		return res;
	}
	else
		return "";
}

void Session::delVerify()
{
	myredis.Hdel("verify_code", ";");
}

bool Session::setLoginName(const std::string & session_id, const std::string & login_name)
{
	if (myredis.Hset(session_id.c_str(), "login_name", login_name.c_str(), ";") != 0) return false;
	return true;
}

int Session::getSessionTTL(const std::string & session_id)
{
	
	return myredis.ttlGet(session_id.c_str());
}

void Session::update_expire_time(std::string sessionid)
{
	myredis.expiresSet(sessionid.c_str(), expire_time);// EXPIRE key seconds
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
