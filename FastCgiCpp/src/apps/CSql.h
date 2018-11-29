#pragma once
#include<TSql.h>
//#include<TMongo.h>
#include<TLog.h>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<TDateTime.h>
#include<map>

#define mongo_tmp_file_path "tmp/"
//错误码
enum 
{
	CSQL_SUCCESS		= 0,//成功
	CSQL_E_CONNECT		= 1,//连接初始化失败
	CSQL_E_LOGIN		= 2,//验证失败
	CSQL_E_QUERRY		= 3, //查询失败
	CSQL_E_UPDATE       = 4, //更新失败
	CSQL_E_PARAM        = 5  //参数错误
};

class CSql
{
public:
	CSql();
	~CSql();
	
	///@brief   验证用户是否登录
	///@param	用户名
	///@return  0 已经登录  其他未登录


	///@brief   初始化类
	///@return  错误码
	int InitConnect(const std::string sIp,
		unsigned short uiPort,
		const std::string sUserName,
		const std::string sPassword,
		const std::string sDatabase);

	///@brief   验证登录
	///@return  错误码
	int UserLogin(const std::string& User, const std::string& Passwd);
	
	///@brief   更新登录ip
	///@return  错误码
	int UpdateLoginIp(const std::string& User, const std::string& Ip);

	///@brief   返回用户信息
	///@return  错误码
	int returnInfo(std::map<std::string, std::string> res, const std::string& User);



	///@brief   查看文件列表
	///@param   user_name			[in] - 用户名  或者用户 ID 提高查询速度  用户ID为登录成功时返回的
	///@param   & vector<string>	[out] - 返回的文件列表 每个[]结构 { 文件名： 文件属性： ，上传日期： 文件大小：}
	///@return  错误码


	///@brief   退出登录 
	///@param   user_name			[in] - 用户名  或者用户 ID 提高查询速度  用户ID为登录成功时返回的
	///@return  错误码


	///@brief   列出库中所有公开的文件 
	///@param   vector<std::string> &			[out] - 存放返回的json 
	///@return  错误码


	///@brief   上传文件 
	///@param   user_name 
	///@param   json			包含：文件名，文件隐私属性，标签 
	///@return  错误码


	///@brief   下载文件 
	///@param   file_name			
	///@param   cla									类别			
	///@param   donw_url							向nginx传递的uri							
	///@return  错误码


	///@brief   删除文件 
	///@param   file_name			
	///@param   cla									类别			
	///@param   donw_url							向nginx传递的uri							
	///@return  错误码
	

private:
	///@brief   上传成功 更新文件对应关系 
	///@param   user_name 
	///@param   file_name			
	///@param   file_size			
	///@return  错误码
	
	WebTool::TMysql SQL;



};

#include<Singleton.h>
/// 定义单例类
typedef Singleton<CSql> CSqlSingle;
#define CSQL CSqlSingle::Instance() //只能在main函数中初始化



