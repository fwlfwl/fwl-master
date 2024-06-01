#ifndef __H_SERVER_CONF__ 
#define __H_SERVER_CONF__ 
/**
 * @brief 服务器配置文件，加载server.yml
 * @details 程序启动需要启动
 * */

#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "../config.h"

namespace fwl{
/**
 * @brief tcp服务配置类
 * */
class TcpConfig{
public:
	typedef std::shared_ptr<TcpConfig> ptr;
	bool operator==(const TcpConfig & in) const{
		return m_addrs == in.m_addrs && 
			m_name == in.m_name &&
			m_timeout == in.m_timeout &&
			m_service_type == in.m_service_type &&
			m_accept_worker == in.m_accept_worker &&		
			m_io_worker == in.m_io_worker &&
			m_process_worker == in.m_process_worker &&
			m_keepalive == in.m_keepalive;
	}
	
	std::vector<std::string> m_addrs;	//服务地址端口
	std::string m_name;	//服务名
	int m_timeout;	//超时时间
	std::string m_service_type;	//服务类型
	bool m_keepalive;
	std::string m_accept_worker;
	std::string m_io_worker;
	std::string m_process_worker;
};

/**
 * @brief LexicalCast偏特化
 * */
template<>
class LexicalCast<std::string, TcpConfig>{
public:
	TcpConfig operator()(const std::string & val){
		YAML::Node node = YAML::Load(val);
		TcpConfig c;
		c.m_name = node["name"].as<std::string>();
		c.m_timeout = node["timeout"].as<int>();
		c.m_service_type = node["type"].as<std::string>();
		c.m_accept_worker = node["accept_worker"].as<std::string>();
		c.m_io_worker = node["io_worker"].as<std::string>();
		c.m_process_worker = node["process_worker"].as<std::string>();
		c.m_keepalive = node["keepalive"].as<bool>();
		if(node["address"].IsDefined()){
			for(size_t i = 0; i < node["address"].size(); ++i){
				c.m_addrs.push_back(node["address"][i].as<std::string>());
			}
		}
		return c;
	}
};

template<>
class LexicalCast<TcpConfig, std::string>{
public:
	std::string operator()(const TcpConfig & c){
		YAML::Node node;
		node["name"] = c.m_name;
		node["timeout"] = c.m_timeout;
		node["type"] = c.m_service_type;
		node["accept_worker"] = c.m_accept_worker;
		node["io_worker"] = c.m_io_worker;
		node["process_worker"] = c.m_process_worker;
		node["keepalive"] = c.m_keepalive;
		for(auto & it : c.m_addrs){
			node["address"].push_back(it);
		}
		std::stringstream ss;
		ss << node;
		return ss.str();
	}
};

}
#endif
