#include "TMongo.h"
#include <mongo/client/dbclient.h>

using namespace mongo;
DBClientConnection * conn = NULL;

namespace WebTool
{
	TMongo::TMongo()
	{
	}
	TMongo::~TMongo()
	{
		if (conn)
			delete conn;
	}

	int TMongo::InitTMongo(const std::string Host)
	{
		conn = new DBClientConnection(false, 0, 3);
		std::string errmsg;
		if (!conn->connect(Host, errmsg))
		{
			cout << "mongo connect err:" << errmsg << endl;
			DBG(L_DEBUG, "mongo初始化失败");
			return 1;
		}
		DBG(L_DEBUG, "mongo初始化成功");
		return 0;
	}

	int TMongo::Upload_mongoFile(const std::string & File_path, const std::string & File_name, const std::string & table, unsigned long long & size)
	{
		GridFS fs(*conn, table);
		fs.storeFile(File_path, File_name);
		size = fs.findFile(File_name).getContentLength();
		return 0;
	}

	int TMongo::Delete_mongoFile(const std::string & File_name, const std::string & table)
	{
		
		GridFS fs(*conn, table);
		fs.removeFile(File_name);

		return 0;
	}
}
//test
//#include <TEncode.h>
//#include <TConf.h>
//#include <TFile.h>
//#include <Template.h>
//#include <TString.h>
//using namespace WebTool;
//static TConf gConf("fcgi.conf");
//int main()
//{
//	TString type = gConf.getConfStr("[LOG]", "type", "file");
//	TString fileName = gConf.getConfStr("[LOG]", "fileName", "fcgi.log");
//	TString ip = gConf.getConfStr("[LOG]", "ip", "127.0.0.1");
//	TString port = gConf.getConfStr("[LOG]", "port", "9090");
//	TString threadCount = gConf.getConfStr("[ROUTE]", "threadCount", "10");
//
//	TLOG->initConfig(L_DEBUG, type, fileName, ip, port.toInt());
//
//	TMongo mongo;
//	std::string Host = "localhost:27017";
//	mongo.InitTMongo(Host);
//	
//}

