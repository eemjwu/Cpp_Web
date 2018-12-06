#pragma once
#include "CProducts_lists.h"
#include "TConvert.h"
#include "TRegex.h"
#include <string>
#include <cstdlib>
#include<stdio.h>

using namespace std;
Response CProducts_lists::products_lists(Request req)
{
	DBG(L_DEBUG, "method : %s,  url: %s,  params: %s, cookie: %s",
		req.getMethod().c_str(),
		req.getUrl().c_str(),
		req.getParams().c_str(),
		req.getCookie().c_str());

	Params reqParams(req.getParams());
	Cookie reqCookie(req.getCookie());
	Response res;
	TString data;

	res.setContentType("application/json");
	if ("GET" == req.getMethod())
	{
		/************************************************/
		// ��ʼ�������ʽ��ǰ��ʹ��jqgrid�б���Ҫָ�������ʽ��
		std::string data_json;
		WebTool::TJson::setInt(data_json, "records", 0);
		WebTool::TJson::setInt(data_json, "total", 0);
		WebTool::TJson::setInt(data_json, "page", 1);
		WebTool::TJson::setArray(data_json, "rows", {});

		/*************************************************/

		// ��ȡ�б�����

		// ���ò�ѯ����
		std::string wheres = "";
		// ��Ʒ���� id
		TConvert T;
		string p_c_id = reqParams["product_class_id"];
		int product_class_id = T.to_int0(p_c_id);
		if (product_class_id > 0)
			wheres = "where id=" + to_string(product_class_id);
		// ҳ������
		int page_number = T.to_int1(reqParams["page"]);
		// ҳ����ʾ��¼����
		int page_size = T.to_int0(reqParams["rows"]);
		// �����ֶ�
		string sidx = reqParams["sidx"];
		// ˳��������
		string sord = reqParams["sord"];
		// ��ʼ�������ֶ�
		string order_by = "";
		if (sidx != "")
			order_by = sidx + " " + sord;

		/************************************************/

		char sql[512];
		sprintf(sql, "select count(1) as records from product %s", wheres.c_str());

		WebTool::TSqlData result;

		// �����ѯʧ�ܻ򲻴���ָ��������¼����ֱ�ӷ��س�ʼֵ
		if (CSQL->execSQL(sql, result) != 0 || result.getData(0, "records") == "0")
		{
			DBG(L_DEBUG, "��ѯʧ�ܻ򲻴���ָ��������¼");
			data = data_json;
			res.setResData(data);
			return res;
		}
		// �����ܼ�¼����
		WebTool::TJson::setInt(data_json, "records", std::stoi(result.getData(0, "records")));

		/*****************************************************************************************/
		/************************���÷�ҳ������ҳ���С******************************************/
		// ���÷�ҳ��С
		if (page_size <= 0)
			page_size = 10;
		// ������ҳ��
		int page_total;
		if (WebTool::TJson::getInt(data_json, "records") % page_size == 0)
			page_total = WebTool::TJson::getInt(data_json, "records") / page_size;
		else
			page_total = WebTool::TJson::getInt(data_json, "records") / page_size + 1;
		// ��¼��ҳ������
		WebTool::TJson::setInt(data_json, "total", page_total);

		// �ж��ύ��ҳ���Ƿ񳬳���Χ
		if (page_number<1 || page_number>page_total)
			page_number = page_total;
		// ��¼��ǰҳ������ֵ
		WebTool::TJson::setInt(data_json, "page", page_number);

		// ���㵱ǰҳ��Ҫ��ʾ�ļ�¼��ʼλ��
		int record_number = (page_number - 1)*page_size;
		// ���ò�ѯ��ҳ����
		std::string paging = " limit " + std::to_string(record_number) + "," + std::to_string(page_size);
		// ��������
		if (order_by == "") order_by = "id asc";

		/***********************************************************************************************************/
		// ���SQL��ѯ���
		sprintf(sql, "select * from product %s order by %s %s", wheres.c_str(), order_by.c_str(), paging.c_str());

		std::vector<std::string> obj;
		if (CSQL->execSQL(sql, result) == 0 && result.getRow() != 0)
		{
			// �洢��¼
			for (int i = 0; i < result.getRow(); i++)
			{
				std::string rowdata_json;
				for (int j = 0; j < result.getCol(); j++)
				{
					WebTool::TJson::setStr(rowdata_json, result.getHead()[j], result.getData(i, j));
				}
				obj.push_back(rowdata_json);
			}

			WebTool::TJson::setArray(data_json, "rows", obj);
		}
		res.setContentType("application/json");

		if (data_json != "")
		{
			data = data_json;
			res.setResData(data);
			return res;
		}
		data = web_helper::return_msg(-1, "��ѯʧ��");
		res.setResData(data);
		return res;
	}
	else if ("POST" == req.getMethod())
	{
		

		// ������¼
		string name = reqParams["name"];
		string code = reqParams["code"];
		TConvert T;
		string p_c_id = reqParams["product_class_id"];
		int product_class_id = T.to_int0(p_c_id);
		string standard = reqParams["standard"];
		string quality_guarantee_period = reqParams["quality_guarantee_period"];
		string place_of_origin = reqParams["place_of_origin"];
		string front_cover_img = reqParams["front_cover_img"];
		string content = reqParams["content"];

		// ��ʱ����
		// ��sqlע�빥������
		/*WebTool::TRegex R;
		content = R.filter_str(content, "'");*/

		// ��xss��������

		int is_enable = T.to_int0(reqParams["is_enable"]);

		char sql[512];
		if (reqParams["_method"] == "" && reqParams["id"] == "")
		{
			//�������
			sprintf(sql, "INSERT INTO `product` (`name`, `code`, `product_class_name`, `standard`, `quality_guarantee_period`, `place_of_origin`, `front_cover_img`, `content`, `is_enable`) values('%s', '%s', '%d', '%s', '%s', '%s', '%s', '%s', '%d')", name.c_str(), code.c_str(), product_class_id, standard.c_str(), quality_guarantee_period.c_str(), place_of_origin.c_str(), front_cover_img.c_str(), content.c_str(), is_enable);
			DBG(L_DEBUG, "sql:%s", sql);
		}
		else 
		{
			if (reqParams["_method"] == "delete")
			{
				// ɾ������
				sprintf(sql, "delete from product where id=%s",reqParams["id"].c_str());
				DBG(L_DEBUG, "sql:%s", sql);
			}
			else 
			{
				// �޸�����
				sprintf(sql, "update product set name = '%s', code = '%s', product_class_name = '%d', standard = '%s', quality_guarantee_period = '%s',place_of_origin = '%s', front_cover_img = '%s', content = '%s', is_enable = '%d' where id = '%s'", name.c_str(), code.c_str(), product_class_id, standard.c_str(), quality_guarantee_period.c_str(), place_of_origin.c_str(), front_cover_img.c_str(), content.c_str(), is_enable, reqParams["id"].c_str());
				DBG(L_DEBUG, "sql:%s", sql);
			}
			
		}
		
		WebTool::TSqlData result;
		if (CSQL->execSQL(sql, result) == 0)
		{
			DBG(L_DEBUG, "�ɹ�");
			std::string msg = WebTool::web_helper::return_msg(0, "success");
			data = msg;
			res.setResData(data);
		}
		else
		{
			DBG(L_DEBUG, "ʧ��");
			std::string msg = WebTool::web_helper::return_msg(-1, "fail");
			DBG(L_DEBUG, "msg%s",msg.c_str());
			data = msg;
			res.setResData(data);
		}
		return res;

	}

}
