#include "ws_server.h"
#include "../log.h"

static fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

namespace fwl{
namespace http{
	/**
	 * client handler
	 * */
	void WsServer::handlerClient(Socket::ptr client){
		WsSession::ptr session(new WsSession(client, true));		
		auto result = session -> handleshake();	
		if(result -> m_state != (int)WsSession::SHARK_STATE::NORMAL || !result -> m_request){
			FWL_LOG_DEBUG(g_logger) << "NeedClose" << ",errno = " << errno << ".strerror=" << strerror(errno);
			return;
		}
		auto req = result -> m_request;
		m_matcher -> handle(req, session);
		FWL_LOG_DEBUG(g_logger) << "Websocket client handle here";
	}
}
}
