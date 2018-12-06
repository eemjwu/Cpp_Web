#include "Route.h"
#include "memory.h"
#include "Request.h"
#include<cstdio>
#include<cstring>

//#define THREAD_COUNT 20
//static int counts[THREAD_COUNT];

Route::Route(int threadNum) :
	m_iThreadNum(threadNum)
{

}

Route::~Route()
{

}

void Route::addRoute(TString requestRoute, FuncHttp func)
{
	m_routeMap.insert(RouteMap::value_type(requestRoute, func));
}

void Route::exec()
{
	if (m_iThreadNum > 1)
	{
		//多线程模式
		pthread_t id[m_iThreadNum];
		FCGX_Init();
		for (int i = 1; i < m_iThreadNum; i++)
		{
			threadStruct p;
			p.pthis = this;
			p.threadID = i;
			pthread_create(&id[i], NULL, pthreadTask, (void*)&p);
		}
		processMessage(0);
	}
	else
	{
		//单线程模式
		while (FCGI_Accept() >= 0)
		{
			//"GET" or "POST"
			TString requestMethod = TString(getenv("REQUEST_METHOD"));
			//"/login?key=value&name=pwd"
			TString requestUri = TString(getenv("REQUEST_URI"));
			size_t pos = requestUri.find_first_of("?");
			TString requestRoute = requestUri.left(pos);
			TString requestParam;
			if ("GET" == requestMethod)
			{
				if (-1 != pos)
				{
					requestParam = requestUri.right(requestUri.length() - pos - 1);
				}
			}
			else if ("POST" == requestMethod)
			{
				int ilen = atoi(getenv("CONTENT_LENGTH")) + 1;

				char *bufpost = (char*)malloc(ilen);
				memset(bufpost, 0, ilen);
				FCGI_fread(bufpost, ilen, 1, FCGI_stdin);
				requestParam = TString(bufpost);
				free(bufpost);
			}

			RouteMap::iterator iterMap;;
			iterMap = m_routeMap.find(requestRoute);
			if (iterMap == m_routeMap.end())
			{
				DBG(L_ERROR, "not find route: %s ", requestUri.c_str());
			}
			else
			{
				FuncHttp func = iterMap->second;
				Request req;
				req.setMethod(requestMethod);
				req.setUrl(requestRoute);
				req.setParams(requestParam);
				req.setCookie(getenv("HTTP_COOKIE"));

				Response res = func(req);

				FCGI_printf(res.Out().c_str());
			}
		}
	}
}

void Route::processMessage(int threadID)
{
	//    pid_t pid = getpid();
	int rc;
	FCGX_Request request;
	FCGX_InitRequest(&request, 0, 0);
	for (;;)
	{
		static pthread_mutex_t accept_mutex = PTHREAD_MUTEX_INITIALIZER;
		//        static pthread_mutex_t counts_mutex = PTHREAD_MUTEX_INITIALIZER;

		pthread_mutex_lock(&accept_mutex);
		rc = FCGX_Accept_r(&request);
		pthread_mutex_unlock(&accept_mutex);
		if (rc < 0)
			break;
		/*********************************************************************************/
		/*********** 读取客户端信息 ******************************************************/
		/*********************************************************************************/

		std::string clientIp;
		if (FCGX_GetParam("X-Forwarded-For", request.envp))
			clientIp = std::string(FCGX_GetParam("X-Forwarded-For", request.envp));

		/*std::string H;
		if (FCGX_GetParam("Host", request.envp))
		H = std::string(FCGX_GetParam("Host", request.envp));
		DBG(L_DEBUG, "Host: %s", H.c_str());*/

		std::string httpCookie;
		if (FCGX_GetParam("HTTP_COOKIE", request.envp))
			httpCookie = std::string(FCGX_GetParam("HTTP_COOKIE", request.envp));

		//"GET" or "POST"
		TString requestMethod;
		if (FCGX_GetParam("REQUEST_METHOD", request.envp))
			requestMethod = TString(FCGX_GetParam("REQUEST_METHOD", request.envp));

		//"/login?key=value&name=pwd"
		TString requestUri;
		if (FCGX_GetParam("REQUEST_URI", request.envp))
			requestUri = TString(FCGX_GetParam("REQUEST_URI", request.envp));

		size_t pos = requestUri.find_first_of("?");
		TString requestRoute = requestUri.left(pos);
		/*********************************************************************************/
		/*********************************************************************************/

		// 过滤不用做任何操作的路由（即过滤不用进行判断是否登录和记录日志的url）
		if ((requestRoute != "/favicon.ico") && (requestRoute != "/") && (requestRoute != "/api/verify/"))
		{
			// 记录客户端提交的参数
			DBG(L_DEBUG, "url: %s ip:%s", requestUri.c_str(), clientIp.c_str());

			// 过滤不用进行登录权限判断的路由（登录与退出登录不用检查是否已经登录）
			if ((requestRoute == "/api/login/") || (requestRoute == "/api/logout/"))
			{
				;
			}
			else
			{
				;//test
				 //已经登录成功的用户session肯定有值，没有值的就是未登录
				 //Cookie cookie(httpCookie);
				 //std::string sessionId = cookie["sessionId"];
				 //if (MySession->existSessionId(sessionId) != 0 || MySession->getSessionTTL(sessionId) < 5)//不存在sessionID  或者剩余时间短 登录已失效 返回登录界面
				 //{
				 //	DBG(L_DEBUG, "登录已失效,返回登录界面");
				 //	TString res;
				 //	res += "X-Accel-Redirect: /login.html\r\n";
				 //	FCGX_PutS(res.c_str(), request.out);
				 //}
			}
		}


		TString requestParam;
		if (requestRoute != "/api/files/")
		{
			/*******************************************************************************************************/
			/*******************************************************************************************************/
			/**************获取用户提交的参数***********************************************************************/
			/*******************************************************************************************************/
			if ("GET" == requestMethod)
			{
				if (-1 != pos)
				{
					requestParam = requestUri.right(requestUri.length() - pos - 1);
				}
			}
			else if ("POST" == requestMethod)
			{
				if (FCGX_GetParam("CONTENT_LENGTH", request.envp))
				{

					int ilen = atoi(FCGX_GetParam("CONTENT_LENGTH", request.envp)) + 1;

					char *bufpost = (char*)malloc(ilen);
					memset(bufpost, 0, ilen);
					FCGX_GetStr(bufpost, ilen, request.in);
					//url 解码
					char *bufres = (char*)malloc(ilen);
					memset(bufres, 0, ilen);
					WebTool::web_helper::urldecode(bufpost, bufres);
					requestParam = TString(bufres);//转为字符！！
					free(bufpost);
					free(bufres);
				}
			}
		}

		RouteMap::iterator iterMap;
		iterMap = m_routeMap.find(requestRoute);
		if (iterMap == m_routeMap.end())
		{
			DBG(L_ERROR, "not find route: %s ", requestUri.c_str());
		}
		else
		{
			// typedef std::function<Response(Request)> FuncHttp;
			FuncHttp func = iterMap->second;
			Request req;
			req.setMethod(requestMethod);
			req.setUrl(requestRoute);
			req.setParams(requestParam);
			req.setCookie(httpCookie);
			req.setIp(clientIp);
			req.setReqStream(&request);//新增获取前端信息流的句柄
			Response res = func(req);
			if (requestRoute != "/api/verify/")
				FCGX_PutS(res.Out().c_str(), request.out);
			else
			{
				FCGX_PutS(res.Out().c_str(), request.out);
				FCGX_PutStr(res.m_byteDate, 17646, request.out);
			}

			//            DBG(L_DEBUG, "res.Out: %s", res.Out().c_str());
		}
		//        sleep(2);
		//        pthread_mutex_lock(&counts_mutex);
		//        DBG(L_DEBUG, "counts: %5d , PID: %d, threadID: %d" , ++counts[threadID], pid, threadID);
		//        pthread_mutex_unlock(&counts_mutex);
		FCGX_Finish_r(&request);
	}
}
