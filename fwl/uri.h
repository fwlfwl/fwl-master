/**
 * @file uri.h
 * @brief parse uri
 * @author fwl
 * @email 1822363660@qq.com
 * @date 2024-3-3
**/

#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include "address.h"

/*
 * @brief define default port
 * **/
#define DEFAULT_PORT_MAP(XX)	\
	XX("ftp", 21)	\
	XX("ssh", 22)	\
	XX("telent", 23)	\
	XX("http", 80)	\
	XX("ws", 80)	\
	XX("https", 443)	\
	XX("wss", 443)	\
	XX("rtsp", 554)	
	//others...

namespace fwl{
/** 
 	foo://user@example.com:8042/over/there?name=ferret#nose
     \_/   \___________________/\_________/ \_________/ \__/
      |              |              |            |        |
   scheme        authority         path        query   fragment
      |   _____________________|__
     / \ /                        \
     urn:example:animal:ferret:nose 
 **/
class Uri{
public:
	typedef std::shared_ptr<Uri> ptr;
	/**
	 * Create uri
	 * */
	static Uri::ptr Create(const std::string & uriStr);

	/**
	 * @brief constructor
	 * */
	Uri() = default;
	//get scheme 
	const std::string &  getScheme() const {	return m_scheme;	}

	//get userinfo 
	const std::string & getUserinfo() const {	return m_userinfo;	}

	//get host 
	const std::string & getHost() const {	return m_host;	}
	
	//get path 
	const std::string & getPath() const {	return m_path;	}

	//get query 
	const std::string & getQuery() const {	return m_query;	}
	
	//get port
	uint32_t getPort() const {	return m_port;	}
	
	//get fragment
	const std::string & getFragment() const {	return m_fragment;	}
	
	//set scheme
	void setScheme(const std::string & scheme) {	m_scheme = scheme;	}

	//set userinfo 
	void setUserinfo(const std::string & userinfo)	{	m_userinfo = userinfo;	}
	
	//set host
	void setHost(const std::string & host)	{	m_host = host;	}
	
	//set path
	void setPath(const std::string & path)	{	m_path = path;	}

	//set port 
	void setPort(uint32_t port) {	m_port = port;	}
	
	//set fragment 
	void setFragment(const std::string & fragment) {	m_fragment = fragment;	}
	
	//set query 
	void setQuery(const std::string & query) {	m_query = query;	}

	/**
	 * @brief streaming output
	 * */
	std::ostream & toStream(std::ostream & os) const;

	/**
	 * @brief String output
	 * */
	std::string toString() const;

	/**
	 * @brief is default port
	 * */
	bool isDefaultPort() const;

	/**
	 * @brief create address by uri
	 * */
	Address::ptr CreateAddress();
private:
	//set default port 
	void setDefaultPort();

	//scheme 
	std::string m_scheme;
	// user info 
	std::string m_userinfo;
	//host 
	std::string m_host;	
	//path 
	std::string m_path;
	//port 
	uint32_t m_port = 0;
	//query 
	std::string m_query;
	//fragment 
	std::string m_fragment;
};

}
