/**
 * @file ws_server.h
 * @brief websocket_server encapsulation
 * @auth fwl
 * @email 1822363660@qq.com 
 * @data 2024-03-29
 * */
#ifndef __H_WS_SERVER__
#define __H_WS_SERVER__ 

#include <memory>
#include "ws_servlet.h"
#include "ws_session.h"
#include "../network_server.h"
#include "../iomanager.h"

namespace fwl{
namespace http{

class WsServer : public NetworkServer{
public:
	typedef std::shared_ptr<WsServer> ptr;
	/**
	 * @brief constructor
	 * */	
	WsServer(const std::string &name = "", IOManager * iom = fwl::IOManager::GetThis(), IOManager * work = fwl::IOManager::GetThis()):
		NetworkServer(name, "TCP", iom, work){
			m_matcher.reset(new WsServletMatcher);
		}

	/**
	 * client handler
	 * */
	void handlerClient(Socket::ptr client) override;

	/**
	 * @brief get ServletMatcher
	 * */
	WsServletMatcher::ptr getServlet() const {	return m_matcher;	}
private:
	//ServletMatcher
	WsServletMatcher::ptr m_matcher = nullptr;
};

}
}
#endif
