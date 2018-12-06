
#include "main.h"

// test
#include<string>
using namespace std;
//
int main(void)
{

	//

	//
	
	TString type = gConf.getConfStr("[LOG]", "type", "file");
	TString fileName = gConf.getConfStr("[LOG]", "fileName", "fcgi.log");
	TString ip = gConf.getConfStr("[LOG]", "ip", "127.0.0.1");
	TString port = gConf.getConfStr("[LOG]", "port", "9090");
	TString threadCount = gConf.getConfStr("[ROUTE]", "threadCount", "10");

	TLOG->initConfig(L_DEBUG, type, fileName, ip, port.toInt());
	

	DBG(L_DEBUG, "===========start=========== type:%s, fileName:%s, ip:%s, port:%s",
		type.c_str(), fileName.c_str(), ip.c_str(), port.c_str());

	if (CSQL->InitConnect("localhost", 3306, "root", "0123698745", "test") != CSQL_SUCCESS)
	{
		DBG(L_DEBUG, "SQL初始化失败");
		exit(1);
	}
	DBG(L_DEBUG, "SQL初始化成功");	
	
	// 初始化 session  过期时间  IP 端口
	if (MySession->InitSession(3600,"127.0.0.1",6379) != 0) 
	{
		DBG(L_DEBUG, "初始化失败");
		exit(1);
	}
	DBG(L_DEBUG, "初始化成功");


	Route appRoute(threadCount.toInt());
	CIndex appIndex;
	appRoute.addRoute("/api/login/", routeBind(CIndex::index, appIndex));  // m_routeMap 添加{ / ， index(appIndex)  }
	appRoute.addRoute("/index/", routeBind(CIndex::index, appIndex));
	CLogin appLogin;
	appRoute.addRoute("/logout", routeBind(CLogin::logout, appLogin));
	appRoute.addRoute("/login", routeBind(CLogin::login, appLogin));
	CArticle appArticle;
	appRoute.addRoute("/api/files/", routeBind(CArticle::articleList, appArticle));//公司介绍修改uri
	CAbout appAbout;
	appRoute.addRoute("/api/about/", routeBind(CAbout::about, appAbout)); //新增公司介绍显示uri

	CProduct appProduct;
	appRoute.addRoute("/api/product_class/", routeBind(CProduct::product, appProduct)); //新增产品介绍

	Verify myverify;
	appRoute.addRoute("/api/verify/", routeBind(Verify::get_verify, myverify));

	CProducts_lists appCProducts_lists;
	appRoute.addRoute("/api/product/", routeBind(CProducts_lists::products_lists, appCProducts_lists));

	appRoute.exec();

	return 0;
}
