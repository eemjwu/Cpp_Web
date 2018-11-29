#pragma once
#include <TString.h>
#include<TRedis.h>
#include<TLog.h>
#include<TJson.h>
#include<unordered_map>
#include<random_helper.h>
using namespace WebTool;

class Session
{
	/* session 条目：
	* ID
	* login_name
	* verify_code
	*/
	/*session 方法：
	* get
	
	*/
public:
	Session() 
	{
		expire_time = 3600;//默认3600秒  1小时
		redis_ip = "127.0.0.1";
		redis_port = 6379;
		if (myredis.InitTRedis(redis_ip, redis_port) == 0)
		{
			DBG(L_DEBUG, "session 初始化成功");
		}
		////应该要抛出异常
		//// throw

	}
	Session(int cookie_expires , const std::string& redis_ip = "127.0.0.1",unsigned short port = 6379) :expire_time(cookie_expires), redis_ip(redis_ip),redis_port(port)
	{
		if (myredis.InitTRedis(redis_ip, redis_port == 0))
		{
			DBG(L_DEBUG, "session 初始化成功");
		}
		//应该要抛出异常
		// throw

	}
	~Session() { myredis.~TRedis(); };

	int InitSession(int cookie_expires = 3600, const std::string& redis_ip = "127.0.0.1", unsigned short port = 6379);

	void set_expire_time(int cookie_expires) { expire_time = cookie_expires; }
	int get_expire_time() { return this->expire_time; }

	bool existSessionId(const std::string& session_id);//是否存在

	std::string getSession(const std::string& session_id);//返回json

	std::string addSessionID(const std::string& conversasionID);//返回本次会话的sessionID  并保存进session

	bool setVerify(WebTool::TString& verify);//因为要忽略大小写 所以会改变传进来的verify

	std::string getVerify();

	void delVerify();

	bool setLoginName(const std::string& session_id, const std::string& login_name);//添加登录用户名

	int getSessionTTL(const std::string& session_id);


private:

	void update_expire_time(std::string sessionid);// 每操作一次，更新当前seesionid的过期时间
	std::string redis_ip = "127.0.0.1";
	unsigned short redis_port = 6379;
	WebTool::TRedis myredis;
	int expire_time;

};
#include<Singleton.h>
/// 定义单例类
typedef Singleton<Session> MySession;
#define MySession MySession::Instance() //只能在main函数中初始化

