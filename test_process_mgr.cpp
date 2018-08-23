#include <fstream>
#include "test_process_mgr.h"
#include "log.hpp"

test_process_mgr_t::test_process_mgr_t(const std::string &config_file)
{	trace_worker();
	if (file_to_json(config_file, m_config_file_value) == false)
	{
		return;
	}
	return;
}

test_process_mgr_t::~test_process_mgr_t()
{	trace_worker();
}

bool test_process_mgr_t::file_to_json(const std::string &file_name, Json::Value &file_name_value)
{	trace_worker();
	Json::Reader reader;

	std::ifstream in(file_name.c_str(), std::ios::binary);
	if(in.is_open() == false)
	{	trace_printf("NULL");
		return false;
	}
	if(reader.parse(in, file_name_value) == false)
	{	trace_printf("NULL");
		return false;
	}
	return true;
}

bool test_process_mgr_t::process_method(const std::string& method_name, const Json::Value &args_value)
{	trace_worker();
	trace_printf("method_name.c_str()  %s", method_name.c_str());
    method_t &methodfunction = m_methodMap[method_name];
    if (methodfunction)
    {
        methodfunction(args_value);
        return true;
    }
	else
	{
		const Json::Value &flow_array_value = args_value["flow"];
		if (flow_array_value.isNull())
		{	trace_printf("false");
			return false;
		}
		for (uint32_t flow_array_index=0; flow_array_index<flow_array_value.size(); ++flow_array_index)
		{
			const Json::Value &flow_item_value = flow_array_value[flow_array_index];
			Json::Value::Members member = flow_item_value.getMemberNames();
			for(Json::Value::Members::iterator iter = member.begin(); iter != member.end(); ++iter)
			{
				method_t &methodfunction = m_methodMap[*iter];
				if (methodfunction)
				{
					methodfunction(flow_item_value);
				}
			}
		}
	}
    return false;
}

void test_process_mgr_t::register_method(const std::string& method_name, method_t method)
{
	trace_printf("method_name.c_str()  %s", method_name.c_str());
    m_methodMap[method_name] = method;
}

void test_process_mgr_t::deal_test()
{	trace_worker();
	if (m_config_file_value == Json::nullValue)
	{
		return;
	}
	//trace_printf(m_config_file_value.toStyledString().c_str());
	std::string method_name;
	const Json::Value &test_item_array_value = m_config_file_value["test_item"];
	for (uint32_t test_item_index=0; test_item_index<test_item_array_value.size(); ++test_item_index)
	{
		const Json::Value &test_item_value = test_item_array_value[test_item_index];
		const Json::Value &method_args_value = test_item_value["method_args"];
		method_name = test_item_value["method_name"].asString();
		
		process_method(method_name + "_start", method_args_value[0u]);
		process_method(method_name + "_process", method_args_value[1]);
		process_method(method_name + "_end", method_args_value[2]);
	}
}

