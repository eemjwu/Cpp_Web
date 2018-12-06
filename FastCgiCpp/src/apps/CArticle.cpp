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
		DBG(L_INFO, "״̬��%d",tmp);*/
		//res_fileName��ǰ�˷��͹������ļ�������Ҫ�Ӵ˴���ȡ��upload������Ҳ��ȡ�ˣ����Ǻ�������ʱ�������ˣ���ȡ��������
		static TString res_fileName;
		int tmp = WebTool::web_helper::upload_file_save_as(*(req.getReqStream()), res_fileName, ospath"/upload/");

		DBG(L_DEBUG, "data_addr = %s", res_fileName.c_str());
		//�����ļ��ĵ�ַ
		std::string data_addr = "/upload/" + res_fileName;
		std::string data_json;
		std::string data_end_json;

		TJson::setStr(data_json, "url", data_addr);
		TJson::setStr(data_json, "state", "SUCCESS");

		TJson::setObj(data_end_json, "data", data_json);


		DBG(L_INFO, "״̬��%d", tmp);
		DBG(L_DEBUG, "data_addr = %s", data_addr.c_str());

		//��string���͵�data_jsonת��Ϊdata
		data = TString(data_end_json);


		res.setSetCookie(reqCookie);
		res.setResData(data);

    }
    return res;
}
