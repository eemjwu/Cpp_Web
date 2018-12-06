#include "CAbout.h"
#include "Session.h"
#include<string>
#include<stdio.h>


//@ /about?
//���ǹ�˾���ܺ���CAbout������������Response�����ͻ��˷������ݿ��������
Response CAbout::about(Request req)
{

	if ("GET" == req.getMethod())
	{
		//...
	    //������ݿ������ݣ�ֱ����ʾ���ݣ������кܶ����ֺ�ͼƬ����ϣ���ʾ���µ�
	    //...
		DBG(L_DEBUG, "�������ݿ��ȡ��������");

		Response res;
		TString data;
		TSqlData sqlData;

		Cookie reqCookie(req.getCookie());



		//��ʼ��ҳ��
		/*TString dataStr;
		dataStr.loadFile(TString(HTML_PATH) + "index.html");

		WebTool::Template temp(TString(HTML_PATH) + "template.html");
		temp.set("newTitle1", "��˾����");
		temp.set("newHtml", dataStr);
		data = temp.toStr();*/



		//��ʼ�����ݿ�����
		if (SQL.InitTMysql("218.192.170.127", 3306, "root", "0123698745", "test") == WebTool::MYSQL_SUCCESS)
			DBG(L_DEBUG, "SQL login success");
		if (SQL.openDB() == WebTool::MYSQL_SUCCESS)
			DBG(L_DEBUG, "SQL open success");

		//��ȡָ����¼
		//��test1������ݿ�ѡ�����е��У�û��whereָ�������е��ж�Ҫ
		std::string sqlcommand = "select * from information";
		SQL.execSQL(sqlcommand, sqlData);
		DBG(L_DEBUG, "SQL read line 1 success");

		//sqlDataΪ��ȡ�����ݣ������µ��ı�չʾ����,�����һ�о�����������
		std::string data_title = sqlData.getData(0, "title");

		DBG(L_DEBUG, "data_title = %s", data_title.c_str());

		std::string data_content = sqlData.getData(0, "content");

		//��ȡͼƬ������ļ�������ֻ��һ��ͼƬ�ͺ���
		//���������ݿ�ֻ��1��ͼƬ��ÿ���ϴ����Ḳ�ǣ���ȡ��ͼƬ
		//�����ݿ��з���ͼƬ��ַ����ǰ�˶���ͼƬ

		std::string data_pic = sqlData.getData(0, "front_cover_img");

		DBG(L_DEBUG, "data_pic = %s", data_pic.c_str());

		//FILE *fp = fopen("/home/wmj.png", "rb");
		//int pic_len;

		//fseek(fp, 0, SEEK_END);  //һֱѰ�ҵ��ļ�β��
		//pic_len = ftell(fp);  //�õ�ͼƬ�ĳ���
		//rewind(fp);  //rewind���ļ�ָ��ָ��ͷ
		//char *pic_buf = new char[pic_len + 1];  //����һ���ռ��ڶ���
		//memset(pic_buf, 0, pic_len + 1);  //����ļ�ָ��
		////��ȡ�ļ�����
		//fread(pic_buf, sizeof(char), pic_len, fp);
		//fclose(fp);
		////��char* pic_buf��������ӵ�Tstring data��
		////TString data_pic;
		////data_pic = TString(pic_buf);

		//std::string data_pic = pic_buf;


		//��title,pic,content�������json����
		std::string data_json;
		std::string data_end_json;
		TJson::setStr(data_json, "title", data_title);
		TJson::setStr(data_json, "front_cover_img", data_pic);
		TJson::setStr(data_json, "content", data_content);

		TJson::setObj(data_end_json, "data", data_json);


		//��string���͵�data_jsonת��Ϊdata
		data = TString(data_end_json);

		//...
		//������ݿ�û�����ݣ���ʾ��Ҫ��������
		//...

		reqCookie.delCookie("password");
		res.setSetCookie(reqCookie);
		res.setResData(data);
		return res;
	}

	
	else if ("POST" == req.getMethod())
	{
		//���ǹ�˾���������޸Ľӿڣ��޸����ݿ���������ݣ������ϴ����֣��ϴ�ͼƬ
        //ͼƬ�Ե�ַ�ķ�ʽ���������ݿ⣬ͼƬ�������ڷ�����
		DBG(L_DEBUG, "�������ݿ��޸Ľ�������");

		//�����������ݵ��ױ������ĸ�������
		DBG(L_DEBUG, "method : %s,  url: %s,  params: %s, cookie: %s",
			req.getMethod().c_str(),
			req.getUrl().c_str(),
			req.getParams().c_str(),
			req.getCookie().c_str());

		Params reqParams(req.getParams());
		Cookie reqCookie(req.getCookie());
		Response res;
		TString data;

		//��ȡ�ϴ������ƣ�ͼƬ�ڷ������ĵ�ַ�����ġ�
		std::string title1 = reqParams["title"];
		std::string front_cover_img1 = reqParams["front_cover_img"];
		std::string content1 = reqParams["content"];

		//��front_cover_img1��%2F�滻��/
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

		//strBase����һͨ�������õ����ַ���
		DBG(L_DEBUG, "front_cover_img1 command %s", strBase.c_str());



		//�������ݿ�
		//��ʼ�����ݿ�����
		if (SQL.InitTMysql("218.192.170.127", 3306, "root", "0123698745", "test") == WebTool::MYSQL_SUCCESS)
			DBG(L_DEBUG, "SQL login success");
		if (SQL.openDB() == WebTool::MYSQL_SUCCESS)
			DBG(L_DEBUG, "SQL open success");

		//���ݿ���¼�¼
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

		//���ݿ��޸ĳɹ�
		/*if (SQL.execSQL(a) == WebTool::MYSQL_SUCCESS)
			DBG(L_DEBUG, "SQL write in success");*/

		return res;

	}

}




//Response About::update
//{
//	//��ǰ�˻�ȡ����,ͼƬͨ��nginx������̨
//	img front_cover_img
//	string content;
//
//

//
//	Params reqParams(req.getParams());
//	//��ȡ��Ƭ,��Ƭ�Ǵ�ǰ��ֱ���ϴ���/tmp�ļ��У���ʱ��������ȡ������
//	front_cover_img
//	//��ȡ����
//	content = reqParams["introduce_text"];
//
//

