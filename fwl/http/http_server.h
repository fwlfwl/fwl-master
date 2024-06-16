/**
 * @file http_server.h
 * @brief http_server encapsulation
 * @auth fwl
 * @email 1822363660@qq.com 
 * @data 2024-02-18
 * */
#ifndef __H_HTTP_SERVER__
#define __H_HTTP_SERVER__ 

#include <memory>
#include "http_servlet.h"
#include "http_session.h"
#include "../network_server.h"
#include "../iomanager.h"

namespace fwl{
namespace http{

class HttpServer : public NetworkServer{
public:
	typedef std::shared_ptr<HttpServer> ptr;
	/**
	 * @brief constructor
	 * */	
	HttpServer(bool isClose  = true, const std::string &name = "", const std::string & type = "TCP",IOManager * iom = fwl::IOManager::GetThis(), IOManager * work = fwl::IOManager::GetThis()):
		NetworkServer(name, type, iom, work),
		m_isClose(isClose){
			m_matcher.reset(new ServletMatcher);
		}

	/**
	 * client handler
	 * */
	void handlerClient(Socket::ptr client) override;

	/**
	 * @brief get close state(keep-alive/close)
	 * */
	bool isClose(){	return m_isClose;	}
	/**
	 * @brief get ServletMatcher
	 * */
	ServletMatcher::ptr getServlet() const {	return m_matcher;	}
private:
	//set close state 
	bool m_isClose;
	//ServletMatcher
	ServletMatcher::ptr m_matcher = nullptr;
};

}
}
#endif
