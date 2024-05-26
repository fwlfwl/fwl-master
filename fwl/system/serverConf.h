#ifndef __H_SERVER_CONF__ 
#define __H_SERVER_CONF__ 
/**
 * @brief 服务器配置文件，加载server.yml
 * @details 程序启动需要启动
 * */

#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>
#include "../config.h"

/**
 * @brief tcp服务配置类
 * */
struct TcpConfig{
	bool operator==(const TcpConfig & in){
		return m_addrs == in.m_addrs && 
			m_name == in.m_name &&
			m_timeout == in.m_timeout &&
			m_serviceType == in.m_serviceType &&
			m_accept_worker == in.m_accept_worker &&		
			m_io_worker == in.m_io_worker &&
			m_process_worker == in.m_process_worker;
	}
	
	std::vector<std::string> m_addrs;	//服务地址端口
	std::string m_name;	//服务名
	int m_timeout;	//超时时间
	std::string m_serviceType;	//服务类型
	std::string m_accept_worker;
	std::string m_io_worker;
	std::string m_process_worker;
}

/**
 * @brief LexicalCast偏特化
 * */
template<>
class LexicalCast<std::string, TcpConfig>{
public:
	TcpConfig operator(const std::string & val){
		YAML::Node node = YAML::load(val);
		TcpConfig c;
		c.m_name = node["name"].as<std::string>();
		c.m_timeout = node["timeout"].as<int>();
		c.m_serviceType = node["type"].as<std::string>();
		c.m_accept_worker = node["accept_worker"].as<std::string>();
		c.m_io_worker = node["io_worker"].as<std::string>();
		c.m_process_worker = node["process_worker"].as<std::string>();
		if(node["address"].isDefined()){
			for(int i = 0; i < node["address"].size(); ++i){
				c.m_addrs.push_back(node["address"][i].as<std::string>());
			}
		}
		return c;
	}
}

#endif
