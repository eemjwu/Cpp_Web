#include "TRedis.h"
#include<string>
#include<iostream>


using namespace std;

namespace WebTool
{

	TRedis::TRedis(const std::string Ip, unsigned short Port): m_sIp(Ip), m_uiPort(Port)
	{
		if (My_redis != NULL) redisFree(My_redis);
		My_redis = redisConnect(m_sIp.c_str(), m_uiPort);
		if (My_redis == NULL || My_redis->err)
		{
			//DBG(L_DEBUG, "redis连接失败");
			cout << "失败" << endl;
			exit(1);
		}
		cout << "成功" << endl;
	}


	int TRedis::InitTRedis(const std::string Ip, unsigned short Port)
	{
		m_sIp = Ip;
		m_uiPort = Port;
		if (My_redis != NULL) redisFree(My_redis);
		My_redis = redisConnect(m_sIp.c_str(), m_uiPort);
		if (My_redis == NULL || My_redis->err)
		{
			//DBG(L_DEBUG, "redis连接失败");
			std::cout << "失败" << endl;
			return -1;
		}
		//DBG(L_DEBUG, "redis连接成功");
		std::cout << "成功" << endl;
		return 0;
	}

	int TRedis::Hget(unordered_map<string, string>& res, const char * key, const char * field, ...)
	{
		// 处理：HGETALL key
		if (field == ";")
		{
			redisReply* reply = (redisReply *)redisCommand(My_redis, "HGETALL %s", key);
			if (reply->type == REDIS_REPLY_ARRAY)//存在数据
			{	
				for (int len = 0; len < reply->elements; len++)
				{
					string key = reply->element[len]->str;
					++len;
					string val = reply->element[len]->str;
					res[key] = val;
					//cout << reply->element[len]->str << endl;
				}
				freeReplyObject(reply);
				return 1;
			}
			//不存在数据
			freeReplyObject(reply);
			return -1;
		}
		else
		{
			va_list pvar;
			va_start(pvar, field);

			char * p = va_arg(pvar, char *);
			if (p== ";")	// 处理：HGET key field
			{
				redisReply* reply = (redisReply *)redisCommand(My_redis, "HGET %s %s", key, field);
				string val;
				if (reply->type != REDIS_REPLY_NIL)
				{
					val = string(reply->str, reply->str + reply->len);
					res[field] = val;
					va_end(pvar);
					freeReplyObject(reply);
					return 0;
				}
				va_end(pvar);
				freeReplyObject(reply);
				return -1;

			}
			else// 处理：HMGET key field1 [field2] 
			{
				vector<char *> param;
				do
				{
					param.push_back(p);
					p = va_arg(pvar, char *);
				} while (p != ";");

				string req = "HMGET " + static_cast<string>(key) + " " +static_cast<string>(field);
				for (int i = 0; i < param.size(); i++)
				{
					req = req + " " + param[i];
				}
				cout << req << endl;
				redisReply* reply = (redisReply *)redisCommand(My_redis,req.c_str());
				if (reply->type == REDIS_REPLY_ARRAY && reply->elements == (param.size() + 1))
				{
					if (reply->element[0]->type != REDIS_REPLY_NIL)
					{
						res[field] = reply->element[0]->str;
					}
					else
					{
						res.clear();
						return -1;
					}
					
					for (int i = 1; i < reply->elements; i++)
					{
						if (reply->element[i]->type != REDIS_REPLY_NIL)
						{
							res[param[i - 1]] = reply->element[i]->str;
						}
						else
						{
							res.clear();
							return -1;
						}
					}
					va_end(pvar);
					freeReplyObject(reply);
					return 0;

				}
				va_end(pvar);
				freeReplyObject(reply);
				return -1;
			}

	
		}
		return -1;
	}

	int TRedis::Hdel(const char * key, const char * field, ...)
	{
		// 处理：HGETALL key
		if (field == ";")
		{
			redisReply* reply = (redisReply *)redisCommand(My_redis, "DEL %s", key);
			if (reply->type == REDIS_REPLY_INTEGER)//存在数据
			{
				if (reply->integer == 1L)
				{
					freeReplyObject(reply);
					return 0;
				}

			}
			//不存在数据
			freeReplyObject(reply);
			return -1;
		}
		else
		{
			va_list pvar;
			va_start(pvar, field);

			char * p = va_arg(pvar, char *);
			if (p == ";")	// 处理：HGET key field
			{
				redisReply* reply = (redisReply *)redisCommand(My_redis, "HDEL %s %s", key, field);
				if (reply->type == REDIS_REPLY_INTEGER)//存在数据
				{
					if (reply->integer == 1L)
					{
						freeReplyObject(reply);
						return 0;
					}

				}
				//不存在数据
				freeReplyObject(reply);
				return -1;

			}
			else// 处理：HMGET key field1 [field2] 
			{
				vector<char *> param;
				do
				{
					param.push_back(p);
					p = va_arg(pvar, char *);
				} while (p != ";");

				string req = "HDEL " + static_cast<string>(key) + " " + static_cast<string>(field);
				for (int i = 0; i < param.size(); i++)
				{
					req = req + " " + param[i];
				}
				cout << req << endl;
				redisReply* reply = (redisReply *)redisCommand(My_redis, req.c_str());
				if (reply->type == REDIS_REPLY_INTEGER)//存在数据
				{
					if (reply->integer == (param.size()+1))
					{
						va_end(pvar);
						freeReplyObject(reply);
						return 0;
					}

				}
				va_end(pvar);
				freeReplyObject(reply);
				return -1;
			}


		}
		return -1;
	}

	int TRedis::existesKey(const char * ID)
	{
		redisReply * reply = (redisReply*)redisCommand(My_redis, "exists %s", ID);
		int res = -1;
		if (reply->type == REDIS_REPLY_INTEGER) {
			if (reply->integer == 1L)
				res = 0;
		}
		freeReplyObject(reply);
		return res;
	}

	int TRedis::Hset(const char * key, const char * field1, const char * val1, const char * field,...)
	{
		// 处理：HSET key field1 val1
		if (field == ";")
		{
			redisReply* reply = (redisReply *)redisCommand(My_redis, "HSET %s %s %s", key,field1,val1);
			if (reply->type != REDIS_REPLY_ERROR)//存在数据
			{
				freeReplyObject(reply);
				return 0;

			}
			//失败
			freeReplyObject(reply);
			return -1;
		}
		else
		{
			vector<char *> param;
			param.push_back(const_cast<char*>(field));
			va_list pvar;
			va_start(pvar, field);

			char * p = va_arg(pvar, char *);
			if (p == ";") return -1;
			do
			{
				param.push_back(p);
				p = va_arg(pvar, char *);
			} while (p != ";");

			string req = "HMSET " + static_cast<string>(key) + " " + static_cast<string>(field1) + " " + static_cast<string>(val1);
			for (int i = 0; i < param.size(); i++)
			{
				req = req + " " + param[i];
			}
			cout << req << endl;
			redisReply* reply = (redisReply *)redisCommand(My_redis, req.c_str());
			if (reply->type != REDIS_REPLY_ERROR)//
			{
				va_end(pvar);
				freeReplyObject(reply);
				return 0;

			}
			va_end(pvar);
			freeReplyObject(reply);
			return -1;

		}
		return -1;
	}


}
#define TEST_MAIN
#ifdef TEST_MAIN

int main()
{
	WebTool::TRedis redis;
	redis.InitTRedis("127.0.0.1", 6379);
	cout << redis.Hset("test", "name", "wmj", "age","18",";") << endl;
}

#endif
