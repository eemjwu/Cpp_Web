#include "CArticle.h"
#include <string>
#define ospath  "/home/Cpp_Web/FastCgiCpp"

///@ /article-list
Response CArticle::articleList(Request req)
{
    DBG(L_DEBUG, "method : %s,  url: %s,  params: %s, cookie: %s",
        req.getMethod().c_str(),
        req.getUrl().c_str(),
        req.getParams().c_str(),
        req.getCookie().c_str());

    Cookie reqCookie(req.getCookie());
    Response res;
    TString data;
    if("GET" == req.getMethod())
    {
		;

    }
    else if("POST" == req.getMethod())
    {
		/*DBG(L_INFO, "52");
		int tmp = WebTool::web_helper::upload_file_save_as(".");
		DBG(L_INFO, "状态：%d",tmp);*/
		//res_fileName是前端发送过来的文件名，需要从此处提取，upload函数内也提取了，但是函数结束时就析构了，提取不出来。
		static TString res_fileName;
		int tmp = WebTool::web_helper::upload_file_save_as(*(req.getReqStream()), res_fileName, ospath"/upload/");

		DBG(L_DEBUG, "data_addr = %s", res_fileName.c_str());
		//返回文件的地址
		std::string data_addr = "/upload/" + res_fileName;
		std::string data_json;
		std::string data_end_json;

		TJson::setStr(data_json, "url", data_addr);
		TJson::setStr(data_json, "state", "SUCCESS");

		TJson::setObj(data_end_json, "data", data_json);


		DBG(L_INFO, "状态：%d", tmp);
		DBG(L_DEBUG, "data_addr = %s", data_addr.c_str());

		//把string类型的data_json转化为data
		data = TString(data_end_json);


		res.setSetCookie(reqCookie);
		res.setResData(data);

    }
    return res;
}
