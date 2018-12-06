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
		// 初始化输出格式（前端使用jqgrid列表，需要指定输出格式）
		std::string data_json;
		WebTool::TJson::setInt(data_json, "records", 0);
		WebTool::TJson::setInt(data_json, "total", 0);
		WebTool::TJson::setInt(data_json, "page", 1);
		WebTool::TJson::setArray(data_json, "rows", {});

		/*************************************************/

		// 获取列表数据

		// 设置查询条件
		std::string wheres = "";
		// 产品分类 id
		TConvert T;
		string p_c_id = reqParams["product_class_id"];
		int product_class_id = T.to_int0(p_c_id);
		if (product_class_id > 0)
			wheres = "where id=" + to_string(product_class_id);
		// 页面索引
		int page_number = T.to_int1(reqParams["page"]);
		// 页面显示记录数量
		int page_size = T.to_int0(reqParams["rows"]);
		// 排序字段
		string sidx = reqParams["sidx"];
		// 顺序还是逆序
		string sord = reqParams["sord"];
		// 初始化排序字段
		string order_by = "";
		if (sidx != "")
			order_by = sidx + " " + sord;

		/************************************************/

		char sql[512];
		sprintf(sql, "select count(1) as records from product %s", wheres.c_str());

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

		// 判断提交的页码是否超出范围
		if (page_number<1 || page_number>page_total)
			page_number = page_total;
		// 记录当前页面索引值
		WebTool::TJson::setInt(data_json, "page", page_number);

		// 计算当前页面要显示的记录起始位置
		int record_number = (page_number - 1)*page_size;
		// 设置查询分页条件
		std::string paging = " limit " + std::to_string(record_number) + "," + std::to_string(page_size);
		// 设置排序
		if (order_by == "") order_by = "id asc";

		/***********************************************************************************************************/
		// 组合SQL查询语句
		sprintf(sql, "select * from product %s order by %s %s", wheres.c_str(), order_by.c_str(), paging.c_str());

		std::vector<std::string> obj;
		if (CSQL->execSQL(sql, result) == 0 && result.getRow() != 0)
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
	else if ("POST" == req.getMethod())
	{
		

		// 新增记录
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

		// 暂时不用
		// 防sql注入攻击处理
		/*WebTool::TRegex R;
		content = R.filter_str(content, "'");*/

		// 防xss攻击处理

		int is_enable = T.to_int0(reqParams["is_enable"]);

		char sql[512];
		if (reqParams["_method"] == "" && reqParams["id"] == "")
		{
			//添加数据
			sprintf(sql, "INSERT INTO `product` (`name`, `code`, `product_class_name`, `standard`, `quality_guarantee_period`, `place_of_origin`, `front_cover_img`, `content`, `is_enable`) values('%s', '%s', '%d', '%s', '%s', '%s', '%s', '%s', '%d')", name.c_str(), code.c_str(), product_class_id, standard.c_str(), quality_guarantee_period.c_str(), place_of_origin.c_str(), front_cover_img.c_str(), content.c_str(), is_enable);
			DBG(L_DEBUG, "sql:%s", sql);
		}
		else 
		{
			if (reqParams["_method"] == "delete")
			{
				// 删除数据
				sprintf(sql, "delete from product where id=%s",reqParams["id"].c_str());
				DBG(L_DEBUG, "sql:%s", sql);
			}
			else 
			{
				// 修改数据
				sprintf(sql, "update product set name = '%s', code = '%s', product_class_name = '%d', standard = '%s', quality_guarantee_period = '%s',place_of_origin = '%s', front_cover_img = '%s', content = '%s', is_enable = '%d' where id = '%s'", name.c_str(), code.c_str(), product_class_id, standard.c_str(), quality_guarantee_period.c_str(), place_of_origin.c_str(), front_cover_img.c_str(), content.c_str(), is_enable, reqParams["id"].c_str());
				DBG(L_DEBUG, "sql:%s", sql);
			}
			
		}
		
		WebTool::TSqlData result;
		if (CSQL->execSQL(sql, result) == 0)
		{
			DBG(L_DEBUG, "成功");
			std::string msg = WebTool::web_helper::return_msg(0, "success");
			data = msg;
			res.setResData(data);
		}
		else
		{
			DBG(L_DEBUG, "失败");
			std::string msg = WebTool::web_helper::return_msg(-1, "fail");
			DBG(L_DEBUG, "msg%s",msg.c_str());
			data = msg;
			res.setResData(data);
		}
		return res;

	}

}
