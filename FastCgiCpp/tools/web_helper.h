#pragma once
#include "ToolGlobal.h"
#include <fcgi_stdio.h>
#include <fcgiapp.h>

#define EUPLOAD_SUCCESS 0 
#define EUPLOAD_NO_DATA -1
#define EUPLOAD_READ    -2
#define EUPLOAD_WRITE   -3
#define DEAL_BUF_LEN 1024
#define SIGN_CODE_LEN 100
#define FILE_NAME_LEN 64
namespace WebTool
{
	enum
	{
		STATE_START,
		STATE_GET_SIGN_CODE,
		STATE_GET_FILE_NAME,
		STATE_GET_FILE_START,
		STATE_GET_FILE_CONTENT,
		STATE_CHECK_END,
		STATE_END
	};
	class web_helper
	{
	public:
		static std::string return_msg(const int& state, const std::string& msg, const std::string& data_json = "");
		static int upload_file_save_as(FCGX_Request& request, TString resfilename, const char * path,const char * newname = "");

	};


}

