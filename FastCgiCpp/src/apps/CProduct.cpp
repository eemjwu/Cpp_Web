#include "CProduct.h"
#include "TConvert.h"
#include <vector>
#include <cstdlib>
#include<stdio.h>
Response CProduct::product(Request req)
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
	std::string data_json;

	if ("GET" == req.getMethod())
	{
		std::string id_edit = reqParams["_id"];
		DBG(L_DEBUG, "id_edit = %s",id_edit.c_str());
		//std::string id_edit = NULL;
		//id_edit = std::stoi(id_edit);
		//�˴�Ϊ��ʼ����ʾ��Ʒ�б�
		if (id_edit.empty())
		{
			/*****************��ȡ�б�����************************/

			WebTool::TConvert T;
			// ҳ������
			std::string page_n = reqParams["page"];
			int page_number = T.to_int1(page_n);
			// ҳ����ʾ��¼����
			std::string page_s = reqParams["rows"];
			int page_size = T.to_int0(page_s);
			// �����ֶ�
			std::string sidx = reqParams["sidx"];
			// ˳���ǵ�������
			std::string sord = reqParams["sord"];
			// ��ʼ�������ֶ�
			std::string order_by = "";
			if (sidx != "")
				order_by = sidx + " " + sord;

			/*************************************************/

			// ��ʼ�������ʽ��ǰ��ʹ��jqgrid�б���Ҫָ�������ʽ��
			std::string data_json;
			WebTool::TJson::setInt(data_json, "records", 0);
			WebTool::TJson::setInt(data_json, "total", 0);
			WebTool::TJson::setInt(data_json, "page", 1);
			WebTool::TJson::setArray(data_json, "rows", {});

			/*************************************************/
			// ִ��sql����ȡָ�������ļ�¼������
			//SELECT 1 FROM ��A; ���:����Ϊ 1 ,��A��X������,�ͻ���ʾX��
			// count(1) ��ͳ�Ʊ��е����еļ�¼���������ֶ�Ϊnull �ļ�¼
			std::string sql = "select count(1) as records from product_class";
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

			// ���㵱ǰҳ��Ҫ��ʾ�ļ�¼��ʼλ��
			int record_number = (page_number - 1)*page_size;
			// ���ò�ѯ��ҳ����
			std::string paging = " limit " + std::to_string(record_number) + "," + std::to_string(page_size);
			// ��������
			if (order_by == "") order_by = "id asc";

			/***********************************************************************************************************/

			// ���SQL��ѯ���

			char tmp[512] = { 0 };
			sprintf(tmp, "select * from product_class order by %s %s", order_by.c_str(), paging.c_str());
			DBG(L_DEBUG, "���:%s", tmp);
			std::vector<std::string> obj;
			if (CSQL->execSQL(tmp, result) == 0 && result.getRow() != 0)
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

		//�˴�Ϊ�༭ʱ����id_edit���ض��б�
		else if (!id_edit.empty())
		{
			DBG(L_DEBUG, "��Ʒ�༭�ӿ�");
			//��ʼ�����ݿ�����
			if (SQL.InitTMysql("218.192.170.127", 3306, "root", "0123698745", "test") == WebTool::MYSQL_SUCCESS)
				DBG(L_DEBUG, "SQL login success");
			if (SQL.openDB() == WebTool::MYSQL_SUCCESS)
				DBG(L_DEBUG, "SQL open success");

			//��ȡ���ݿ�id_edit����
			//�������ݿ����
			char tmp[512] = { 0 };
			std::sprintf(tmp, "select * from product_class where id = %d", std::stoi(id_edit));
			WebTool::TSqlData result;
			//std::sprintf(tmp, "INSERT INTO `test`.`product_class`(`id`, `name`, `is_enable`) VALUES (26, '%s', 1)",name.c_str());


			DBG(L_DEBUG, "SQL command %s", tmp);

			int b;
			b = SQL.execSQL(tmp,result);
			DBG(L_DEBUG, "SQL error in %d", b);
			
			//ȡ�����ݺ󣬽�������name,is_enableΪ�ؼ��ֵ�Json��ʽ����		
			// �洢��¼			
			std::string data_json;
			for (int j = 1; j < result.getCol()-1; j++)
			{
				//ֻ��Ҫ��name,is_enableΪ�ؼ��ֵ�json��ʽ����
				WebTool::TJson::setStr(data_json, result.getHead()[j], result.getData(0, j));
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
		

	}
	else if ("POST" == req.getMethod())
	{
		Params reqParams(req.getParams());
		if ("delete" != reqParams["_method"])
		{
			std::string id_edit = reqParams["_id"];


			//��Ʒ�������ӿڣ�����ӣ��༭��ɾ������
			DBG(L_DEBUG, "��Ʒ��ӽӿ�");

			//�����ϴ������ݵ��ױ������ĸ�������
			DBG(L_DEBUG, "method : %s,  url: %s,  params: %s, cookie: %s",
				req.getMethod().c_str(),
				req.getUrl().c_str(),
				req.getParams().c_str(),
				req.getCookie().c_str());


			Cookie reqCookie(req.getCookie());
			TString data;

			//��¼��ǰidֵ����������id��
			int id_num;

			//��ȡ�ϴ��ķ������ƣ��Ƿ�����
			std::string name = reqParams["name"];
			std::string is_enable = reqParams["is_enable"];

			DBG(L_DEBUG, "name %s", name.c_str());
			//is_enable = std::stoi(is_enable);

			//�������ݿ�
			//��ʼ�����ݿ�����
			if (SQL.InitTMysql("218.192.170.127", 3306, "root", "0123698745", "test") == WebTool::MYSQL_SUCCESS)
				DBG(L_DEBUG, "SQL login success");
			if (SQL.openDB() == WebTool::MYSQL_SUCCESS)
				DBG(L_DEBUG, "SQL open success");

			//���ݿ���¼�¼����Ӽ�¼
			//�鿴���ݿ�ļ�¼�����õ���ǰid

			std::string sql = "select count(1) as records from product_class";
			WebTool::TSqlData result;

			//if (SQL.execSQL(sql, result) == 0 && result.getData(0, "records") != "0")
			//{
			//	DBG(L_DEBUG, "��ѯ�ɹ������ݿ�������");
			//	//��¼��ǰ��id����
			//	id_num = std::stoi(result.getData(0, "records"));
			//	DBG(L_DEBUG, "id_num %d", id_num);
			//	id_num = id_num + 1;

			//}

			//��¼��ǰ��id����
			//int id_num = std::stoi(result.getData(0, "records"));
			//DBG(L_DEBUG, "id_num %d", id_num);
			//id_num = id_num + 1;

			char tmp[512] = { 0 };
			char tmp1[512] = { 0 };
			//��Ӳ�Ʒû��id����add
			if (id_edit.empty())
			{
				std::sprintf(tmp, "INSERT INTO `test`.`product_class`(`name`, `is_enable`) VALUES ('%s', %d)",name.c_str(), std::stoi(is_enable));
			}

			//��Ӳ�Ʒ��id,��edit
			else if (!id_edit.empty())
			{
				std::sprintf(tmp, "UPDATE `test`.`product_class` SET `name` = '%s' WHERE `id` = %d", name.c_str(), std::stoi(id_edit));
				std::sprintf(tmp1, "UPDATE `test`.`product_class` SET `is_enable` = '%d' WHERE `id` = %d", std::stoi(is_enable), std::stoi(id_edit));
			}

			//std::sprintf(tmp, "INSERT INTO `test`.`product_class`(`id`, `name`, `is_enable`) VALUES (26, '%s', 1)",name.c_str());


			DBG(L_DEBUG, "SQL command %s", tmp);
			DBG(L_DEBUG, "SQL command %s", tmp1);

			//���ݿ�ִ�н��
			int b;
			b = SQL.execSQL(tmp);
			DBG(L_DEBUG, "SQL error in %d", b);
			b = SQL.execSQL(tmp1);
			DBG(L_DEBUG, "SQL error in %d", b);

			//�༭�ӿ�ѡ����id����̨ͨ��data�﷢��id����

			//����json
			TJson::setStr(data_json, "jieguo", "success");
			data = TString(data_json);
			res.setResData(data);
		}
		
		else if ("delete" == reqParams["_method"])
		{
			DBG(L_DEBUG, "��Ʒɾ���ӿ�");
			//ɾ��id��

			std::string id_delete = reqParams["_id"];

			Cookie reqCookie(req.getCookie());
			TString data;

			//��ʼ�����ݿ�����
			if (SQL.InitTMysql("218.192.170.127", 3306, "root", "0123698745", "test") == WebTool::MYSQL_SUCCESS)
				DBG(L_DEBUG, "SQL login success");
			if (SQL.openDB() == WebTool::MYSQL_SUCCESS)
				DBG(L_DEBUG, "SQL open success");


			char tmp[512] = { 0 };
			std::sprintf(tmp, "DELETE FROM `test`.`product_class` WHERE `id` = %d", std::stoi(id_delete));
			DBG(L_DEBUG, "SQL command %s", tmp);
			//���ݿ�ִ�н��
			int b;
			b = SQL.execSQL(tmp);
			DBG(L_DEBUG, "SQL error in %d", b);

			//����json
			TJson::setStr(data_json, "jieguo", "success");
			data = TString(data_json);
			res.setResData(data);

		}
		

	}
	return res;
}