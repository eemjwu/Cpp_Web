#include "CAbout.h"
#include "Session.h"
#include<string>
#include<stdio.h>


//@ /about?
//这是公司介绍函数CAbout，返回类型是Response，给客户端返回数据库里的内容
Response CAbout::about(Request req)
{

	if ("GET" == req.getMethod())
	{
		//...
	    //如果数据库有内容，直接显示内容，可能有很多文字和图片的组合，显示最新的
	    //...
		DBG(L_DEBUG, "调用数据库获取介绍内容");

		Response res;
		TString data;
		TSqlData sqlData;

		Cookie reqCookie(req.getCookie());



		//初始化页面
		/*TString dataStr;
		dataStr.loadFile(TString(HTML_PATH) + "index.html");

		WebTool::Template temp(TString(HTML_PATH) + "template.html");
		temp.set("newTitle1", "公司介绍");
		temp.set("newHtml", dataStr);
		data = temp.toStr();*/



		//初始化数据库链接
		if (SQL.InitTMysql("218.192.170.127", 3306, "root", "0123698745", "test") == WebTool::MYSQL_SUCCESS)
			DBG(L_DEBUG, "SQL login success");
		if (SQL.openDB() == WebTool::MYSQL_SUCCESS)
			DBG(L_DEBUG, "SQL open success");

		//获取指定纪录
		//从test1这个数据库选出所有的列，没有where指的是所有的行都要
		std::string sqlcommand = "select * from information";
		SQL.execSQL(sqlcommand, sqlData);
		DBG(L_DEBUG, "SQL read line 1 success");

		//sqlData为获取的内容，把最新的文本展示出来,假设第一行就是最新数据
		std::string data_title = sqlData.getData(0, "title");

		DBG(L_DEBUG, "data_title = %s", data_title.c_str());

		std::string data_content = sqlData.getData(0, "content");

		//获取图片，如果文件夹里面只有一个图片就好了
		//服务器数据库只有1张图片，每次上传都会覆盖，提取此图片
		//从数据库中返回图片地址，从前端读出图片

		std::string data_pic = sqlData.getData(0, "front_cover_img");

		DBG(L_DEBUG, "data_pic = %s", data_pic.c_str());

		//FILE *fp = fopen("/home/wmj.png", "rb");
		//int pic_len;

		//fseek(fp, 0, SEEK_END);  //一直寻找到文件尾部
		//pic_len = ftell(fp);  //得到图片的长度
		//rewind(fp);  //rewind将文件指针指向开头
		//char *pic_buf = new char[pic_len + 1];  //开辟一个空间在堆上
		//memset(pic_buf, 0, pic_len + 1);  //清空文件指针
		////读取文件内容
		//fread(pic_buf, sizeof(char), pic_len, fp);
		//fclose(fp);
		////把char* pic_buf的内容添加到Tstring data里
		////TString data_pic;
		////data_pic = TString(pic_buf);

		//std::string data_pic = pic_buf;


		//把title,pic,content依次组成json返回
		std::string data_json;
		std::string data_end_json;
		TJson::setStr(data_json, "title", data_title);
		TJson::setStr(data_json, "front_cover_img", data_pic);
		TJson::setStr(data_json, "content", data_content);

		TJson::setObj(data_end_json, "data", data_json);


		//把string类型的data_json转化为data
		data = TString(data_end_json);

		//...
		//如果数据库没有内容，显示需要输入内容
		//...

		reqCookie.delCookie("password");
		res.setSetCookie(reqCookie);
		res.setResData(data);
		return res;
	}

	
	else if ("POST" == req.getMethod())
	{
		//这是公司介绍内容修改接口，修改数据库里面的内容，可以上传文字，上传图片
        //图片以地址的方式保存在数据库，图片本身保存在服务器
		DBG(L_DEBUG, "调用数据库修改介绍内容");

		//看看介绍内容到底保存在哪个变量里
		DBG(L_DEBUG, "method : %s,  url: %s,  params: %s, cookie: %s",
			req.getMethod().c_str(),
			req.getUrl().c_str(),
			req.getParams().c_str(),
			req.getCookie().c_str());

		Params reqParams(req.getParams());
		Cookie reqCookie(req.getCookie());
		Response res;
		TString data;

		//读取上传的名称，图片在服务器的地址，正文。
		std::string title1 = reqParams["title"];
		std::string front_cover_img1 = reqParams["front_cover_img"];
		std::string content1 = reqParams["content"];

		//把front_cover_img1的%2F替换成/
		std::string strBase = front_cover_img1;
		std::string strSrc = "%2F";
		std::string strDst = "/";

		std::string::size_type position = 0;
		std::string::size_type srcLen = strSrc.size();
		std::string::size_type dstLen = strDst.size();

		while ((position = strBase.find(strSrc, position)) != std::string::npos)
		{
			strBase.replace(position, srcLen, strDst);
			position += dstLen;
		}

		//strBase经过一通操作，得到新字符串
		DBG(L_DEBUG, "front_cover_img1 command %s", strBase.c_str());



		//传入数据库
		//初始化数据库链接
		if (SQL.InitTMysql("218.192.170.127", 3306, "root", "0123698745", "test") == WebTool::MYSQL_SUCCESS)
			DBG(L_DEBUG, "SQL login success");
		if (SQL.openDB() == WebTool::MYSQL_SUCCESS)
			DBG(L_DEBUG, "SQL open success");

		//数据库更新纪录
		//std::string a = "'UPDATE information SET front_cover_img=%s, content=%s WHERE id = 1',front_cover_img1.c_str(),content1.c_str()";
		char tmp[512] = { 0 };
		char tmp1[512] = { 0 };
		//sprintf_s(tmp, "select * from product_class order by '%s' '%s'", order_by.c_str(), paging.c_str());
		std::sprintf(tmp, "UPDATE `test`.`information` SET `content` = '%s' WHERE `id` = 1", content1.c_str());
		std::sprintf(tmp1, "UPDATE `test`.`information` SET `front_cover_img` = '%s' WHERE `id` = 1", strBase.c_str());

		//std::string a = "UPDATE `test`.`information` SET `content` = 'content1' WHERE `id` = 1";
		//std::string a = "UPDATE `test`.`information` SET `content` = 'pi' WHERE `id` = 1";
		DBG(L_DEBUG, "SQL command %s", tmp);
		DBG(L_DEBUG, "SQL command %s", tmp1);

		int b;
		b = SQL.execSQL(tmp);
		DBG(L_DEBUG, "SQL error in %d", b);
		b = SQL.execSQL(tmp1);
		DBG(L_DEBUG, "SQL error in %d", b);

		//数据库修改成功
		/*if (SQL.execSQL(a) == WebTool::MYSQL_SUCCESS)
			DBG(L_DEBUG, "SQL write in success");*/

		return res;

	}

}




//Response About::update
//{
//	//从前端获取文字,图片通过nginx传到后台
//	img front_cover_img
//	string content;
//
//

//
//	Params reqParams(req.getParams());
//	//获取照片,照片是从前端直接上传到/tmp文件夹，到时候在这里取就行了
//	front_cover_img
//	//获取内容
//	content = reqParams["introduce_text"];
//
//

