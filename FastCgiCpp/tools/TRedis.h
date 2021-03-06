#pragma once

//#include "ToolGlobal.h"
//#include<hiredis-master/hiredis.h>
#include<hiredis.h>
#include<string>
#include<stdio.h>
#include<map>
#include<unordered_map>
#include<vector>
//可变参数标准宏头文件
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

		// hash命令

		//查询多个 field 必须以";"结尾
		int Hget(std::unordered_map<std::string, std::string> & res, const char* key, const char * field = ";", ...);//HGET key field   	HGETALL key  	HMGET key field1 [field2] 

		//删除一个或多个哈希表字段 多个 field 必须以";"结尾
		int Hdel(const char* key, const char * field = ";", ...);

		int existesKey(const char * ID);

		int Hset(const char* key, const char * field1, const char * val1, const char * field = ";",...);

		int Set(const char* key, const char * val);

		int Get(std::string &res, const char* key);
		// 设置过期时间
		int expiresSet(const char* key, int seconds);

		// 查询过期时间 返回剩余的秒数
		// 查询失败返回 -1
		int ttlGet(const char* key);

	private:
		redisContext * My_redis = NULL;//连接句柄
		std::string m_sIp = "127.0.0.1";
		unsigned short m_uiPort;

	};

}


