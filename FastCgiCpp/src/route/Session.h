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
	/* session ��Ŀ��
	* ID
	* login_name
	* verify_code
	*/
	/*session ������
	* get
	
	*/
public:
	Session() 
	{
		expire_time = 3600;//Ĭ��3600��  1Сʱ
		redis_ip = "127.0.0.1";
		redis_port = 6379;
		if (myredis.InitTRedis(redis_ip, redis_port) == 0)
		{
			DBG(L_DEBUG, "session ��ʼ���ɹ�");
		}
		////Ӧ��Ҫ�׳��쳣
		//// throw

	}
	Session(int cookie_expires , const std::string& redis_ip = "127.0.0.1",unsigned short port = 6379) :expire_time(cookie_expires), redis_ip(redis_ip),redis_port(port)
	{
		if (myredis.InitTRedis(redis_ip, redis_port == 0))
		{
			DBG(L_DEBUG, "session ��ʼ���ɹ�");
		}
		//Ӧ��Ҫ�׳��쳣
		// throw

	}
	~Session() { myredis.~TRedis(); };

	int InitSession(int cookie_expires = 3600, const std::string& redis_ip = "127.0.0.1", unsigned short port = 6379);

	void set_expire_time(int cookie_expires) { expire_time = cookie_expires; }
	int get_expire_time() { return this->expire_time; }

	bool existSessionId(const std::string& session_id);//�Ƿ����

	std::string getSession(const std::string& session_id);//����json

	std::string addSessionID(const std::string& conversasionID);//���ر��λỰ��sessionID  �������session

	bool setVerify(WebTool::TString& verify);//��ΪҪ���Դ�Сд ���Ի�ı䴫������verify

	std::string getVerify();

	void delVerify();

	bool setLoginName(const std::string& session_id, const std::string& login_name);//��ӵ�¼�û���

	int getSessionTTL(const std::string& session_id);


private:

	void update_expire_time(std::string sessionid);// ÿ����һ�Σ����µ�ǰseesionid�Ĺ���ʱ��
	std::string redis_ip = "127.0.0.1";
	unsigned short redis_port = 6379;
	WebTool::TRedis myredis;
	int expire_time;

};
#include<Singleton.h>
/// ���嵥����
typedef Singleton<Session> MySession;
#define MySession MySession::Instance() //ֻ����main�����г�ʼ��

