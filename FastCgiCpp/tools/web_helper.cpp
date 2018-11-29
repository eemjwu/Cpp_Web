#include "web_helper.h"

namespace WebTool
{
	std::string web_helper::return_msg(const int& state, const std::string& msg, const std::string& data_json)
	{
		std::string res_json;

		TJson::setInt(res_json, "state", state);
		TJson::setStr(res_json, "msg", msg);
		TJson::setObj(res_json, "data", data_json);

		return res_json;
	}
}

