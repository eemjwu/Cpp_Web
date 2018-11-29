#pragma once

//#include "ToolGlobal.h"
//#include<hiredis-master/hiredis.h>
#include<hiredis.h>
#include<string>
#include<stdio.h>
#include<map>
#include<unordered_map>
#include<vector>
//�ɱ������׼��ͷ�ļ�
#include "stdarg.h"

namespace WebTool
{
	class TRedis
	{
	public:
		TRedis(const std::string Ip, unsigned short Port = 6379);
		TRedis() = default;
		~TRedis() { redisFree(this->My_redis); };
		int InitTRedis(const std::string Ip, unsigned short Port = 6379);

		void CloseRedis(redisContext * My_redis) { redisFree(My_redis); }

		// hash����

		//��ѯ��� field ������";"��β
		int Hget(std::unordered_map<std::string, std::string> & res, const char* key, const char * field = ";", ...);//HGET key field   	HGETALL key  	HMGET key field1 [field2] 

		//ɾ��һ��������ϣ���ֶ� ��� field ������";"��β
		int Hdel(const char* key, const char * field = ";", ...);

		int existesKey(const char * ID);

		int Hset(const char* key, const char * field1, const char * val1, const char * field = ";",...);

		int Set(const char* key, const char * val);

		int Get(std::string &res, const char* key);
		// ���ù���ʱ��
		int expiresSet(const char* key, int seconds);

		// ��ѯ����ʱ�� ����ʣ�������
		// ��ѯʧ�ܷ��� -1
		int ttlGet(const char* key);

	private:
		redisContext * My_redis = NULL;//���Ӿ��
		std::string m_sIp = "127.0.0.1";
		unsigned short m_uiPort;

	};

}


