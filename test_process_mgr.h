/****************************************************************************
created:	2018/4/23
filename: 	test_process_mgr.h
author:		hugh
conatact:	hugh.huang@zoom.us

purpose:	
describe:	
****************************************************************************/
#ifndef __TEST_PROCESS_MGR__
#define __TEST_PROCESS_MGR__
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "json/json.h"
#include "log.hpp"


class test_process_mgr_t
{
public:
	typedef boost::function<void(const Json::Value &)> method_t;
	test_process_mgr_t(const std::string &config_file);
	~test_process_mgr_t();
public:
	void deal_test();
	void register_method(const std::string& method_name, method_t method);
	bool process_method(const std::string& method_name, const Json::Value &args_value);
private:
	bool file_to_json(const std::string &file_name, Json::Value &file_name_value);
private:
	typedef std::map<std::string, method_t> method_map_t;
	Json::Value m_config_file_value;
    method_map_t m_methodMap;
};

#endif

