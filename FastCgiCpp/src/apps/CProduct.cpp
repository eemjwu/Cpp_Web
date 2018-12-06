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
		//此处为初始化显示产品列表
		if (id_edit.empty())
		{
			/*****************获取列表数据************************/

			WebTool::TConvert T;
			// 页面索引
			std::string page_n = reqParams["page"];
			int page_number = T.to_int1(page_n);
			// 页面显示记录数量
			std::string page_s = reqParams["rows"];
			int page_size = T.to_int0(page_s);
			// 排序字段
			std::string sidx = reqParams["sidx"];
			// 顺序还是倒序排序
			std::string sord = reqParams["sord"];
			// 初始化排序字段
			std::string order_by = "";
			if (sidx != "")
				order_by = sidx + " " + sord;

			/*************************************************/

			// 初始化输出格式（前端使用jqgrid列表，需要指定输出格式）
			std::string data_json;
			WebTool::TJson::setInt(data_json, "records", 0);
			WebTool::TJson::setInt(data_json, "total", 0);
			WebTool::TJson::setInt(data_json, "page", 1);
			WebTool::TJson::setArray(data_json, "rows", {});

			/*************************************************/
			// 执行sql，获取指定条件的记录总数量
			//SELECT 1 FROM 表A; 输出:列名为 1 ,表A有X条数据,就会显示X行
			// count(1) 会统计表中的所有的记录数，包含字段为null 的记录
			std::string sql = "select count(1) as records from product_class";
			WebTool::TSqlData result;

			// 如果查询失败或不存在指定条件记录，则直接返回初始值
			if (CSQL->execSQL(sql, result) != 0 || result.getData(0, "records") == "0")
			{
				DBG(L_DEBUG, "查询失败或不存在指定条件记录");
				data = data_json;
				res.setResData(data);
				return res;
			}
			// 保存总记录数量
			WebTool::TJson::setInt(data_json, "records", std::stoi(result.getData(0, "records")));

			/*****************************************************************************************/
			/************************设置分页索引与页面大小******************************************/
			// 设置分页大小
			if (page_size <= 0)
				page_size = 10;
			// 计算总页数
			int page_total;
			if (WebTool::TJson::getInt(data_json, "records") % page_size == 0)
				page_total = WebTool::TJson::getInt(data_json, "records") / page_size;
			else
				page_total = WebTool::TJson::getInt(data_json, "records") / page_size + 1;
			// 记录总页面数量
			WebTool::TJson::setInt(data_json, "total", page_total);

			// 计算当前页面要显示的记录起始位置
			int record_number = (page_number - 1)*page_size;
			// 设置查询分页条件
			std::string paging = " limit " + std::to_string(record_number) + "," + std::to_string(page_size);
			// 设置排序
			if (order_by == "") order_by = "id asc";

			/***********************************************************************************************************/

			// 组合SQL查询语句

			char tmp[512] = { 0 };
			sprintf(tmp, "select * from product_class order by %s %s", order_by.c_str(), paging.c_str());
			DBG(L_DEBUG, "语句:%s", tmp);
			std::vector<std::string> obj;
			if (CSQL->execSQL(tmp, result) == 0 && result.getRow() != 0)
			{
				// 存储记录
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
			data = web_helper::return_msg(-1, "查询失败");
			res.setResData(data);
			return res;
		}

		//此处为编辑时返回id_edit的特定列表
		else if (!id_edit.empty())
		{
			DBG(L_DEBUG, "产品编辑接口");
			//初始化数据库链接
			if (SQL.InitTMysql("218.192.170.127", 3306, "root", "0123698745", "test") == WebTool::MYSQL_SUCCESS)
				DBG(L_DEBUG, "SQL login success");
			if (SQL.openDB() == WebTool::MYSQL_SUCCESS)
				DBG(L_DEBUG, "SQL open success");

			//提取数据库id_edit数据
			//生成数据库代码
			char tmp[512] = { 0 };
			std::sprintf(tmp, "select * from product_class where id = %d", std::stoi(id_edit));
			WebTool::TSqlData result;
			//std::sprintf(tmp, "INSERT INTO `test`.`product_class`(`id`, `name`, `is_enable`) VALUES (26, '%s', 1)",name.c_str());


			DBG(L_DEBUG, "SQL command %s", tmp);

			int b;
			b = SQL.execSQL(tmp,result);
			DBG(L_DEBUG, "SQL error in %d", b);
			
			//取出数据后，将数据以name,is_enable为关键字的Json格式返回		
			// 存储记录			
			std::string data_json;
			for (int j = 1; j < result.getCol()-1; j++)
			{
				//只需要给name,is_enable为关键字的json格式返回
				WebTool::TJson::setStr(data_json, result.getHead()[j], result.getData(0, j));
			}
				
			
			res.setContentType("application/json");
			if (data_json != "")
			{
				data = data_json;

				res.setResData(data);
				return res;
			}
			data = web_helper::return_msg(-1, "查询失败");
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


			//产品分类管理接口，有添加，编辑，删除功能
			DBG(L_DEBUG, "产品添加接口");

			//看看上传的内容到底保存在哪个变量里
			DBG(L_DEBUG, "method : %s,  url: %s,  params: %s, cookie: %s",
				req.getMethod().c_str(),
				req.getUrl().c_str(),
				req.getParams().c_str(),
				req.getCookie().c_str());


			Cookie reqCookie(req.getCookie());
			TString data;

			//记录当前id值，给新增的id用
			int id_num;

			//读取上传的分类名称，是否启用
			std::string name = reqParams["name"];
			std::string is_enable = reqParams["is_enable"];

			DBG(L_DEBUG, "name %s", name.c_str());
			//is_enable = std::stoi(is_enable);

			//传入数据库
			//初始化数据库链接
			if (SQL.InitTMysql("218.192.170.127", 3306, "root", "0123698745", "test") == WebTool::MYSQL_SUCCESS)
				DBG(L_DEBUG, "SQL login success");
			if (SQL.openDB() == WebTool::MYSQL_SUCCESS)
				DBG(L_DEBUG, "SQL open success");

			//数据库更新记录，添加记录
			//查看数据库的记录数，得到当前id

			std::string sql = "select count(1) as records from product_class";
			WebTool::TSqlData result;

			//if (SQL.execSQL(sql, result) == 0 && result.getData(0, "records") != "0")
			//{
			//	DBG(L_DEBUG, "查询成功且数据库有内容");
			//	//记录当前的id总数
			//	id_num = std::stoi(result.getData(0, "records"));
			//	DBG(L_DEBUG, "id_num %d", id_num);
			//	id_num = id_num + 1;

			//}

			//记录当前的id总数
			//int id_num = std::stoi(result.getData(0, "records"));
			//DBG(L_DEBUG, "id_num %d", id_num);
			//id_num = id_num + 1;

			char tmp[512] = { 0 };
			char tmp1[512] = { 0 };
			//添加产品没有id，是add
			if (id_edit.empty())
			{
				std::sprintf(tmp, "INSERT INTO `test`.`product_class`(`name`, `is_enable`) VALUES ('%s', %d)",name.c_str(), std::stoi(is_enable));
			}

			//添加产品有id,是edit
			else if (!id_edit.empty())
			{
				std::sprintf(tmp, "UPDATE `test`.`product_class` SET `name` = '%s' WHERE `id` = %d", name.c_str(), std::stoi(id_edit));
				std::sprintf(tmp1, "UPDATE `test`.`product_class` SET `is_enable` = '%d' WHERE `id` = %d", std::stoi(is_enable), std::stoi(id_edit));
			}

			//std::sprintf(tmp, "INSERT INTO `test`.`product_class`(`id`, `name`, `is_enable`) VALUES (26, '%s', 1)",name.c_str());


			DBG(L_DEBUG, "SQL command %s", tmp);
			DBG(L_DEBUG, "SQL command %s", tmp1);

			//数据库执行结果
			int b;
			b = SQL.execSQL(tmp);
			DBG(L_DEBUG, "SQL error in %d", b);
			b = SQL.execSQL(tmp1);
			DBG(L_DEBUG, "SQL error in %d", b);

			//编辑接口选定了id，后台通过data里发了id过来

			//返回json
			TJson::setStr(data_json, "jieguo", "success");
			data = TString(data_json);
			res.setResData(data);
		}
		
		else if ("delete" == reqParams["_method"])
		{
			DBG(L_DEBUG, "产品删除接口");
			//删除id项

			std::string id_delete = reqParams["_id"];

			Cookie reqCookie(req.getCookie());
			TString data;

			//初始化数据库链接
			if (SQL.InitTMysql("218.192.170.127", 3306, "root", "0123698745", "test") == WebTool::MYSQL_SUCCESS)
				DBG(L_DEBUG, "SQL login success");
			if (SQL.openDB() == WebTool::MYSQL_SUCCESS)
				DBG(L_DEBUG, "SQL open success");


			char tmp[512] = { 0 };
			std::sprintf(tmp, "DELETE FROM `test`.`product_class` WHERE `id` = %d", std::stoi(id_delete));
			DBG(L_DEBUG, "SQL command %s", tmp);
			//数据库执行结果
			int b;
			b = SQL.execSQL(tmp);
			DBG(L_DEBUG, "SQL error in %d", b);

			//返回json
			TJson::setStr(data_json, "jieguo", "success");
			data = TString(data_json);
			res.setResData(data);

		}
		

	}
	return res;
}