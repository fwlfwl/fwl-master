#include "ws_server.h"
#include "../log.h"

namespace fwl{
namespace http{
	/**
	 * client handler
	 * */
	void WsServer::handlerClient(Socket::ptr client){
		WsSession::ptr session(new WsSession(client, true));		
		do{
			auto result = session -> handleshake();	
			if(result -> m_state != (int)WsSession::SHARK_STATE::NORMAL || result -> m_request){
				break;
			}
			auto req = result -> m_request;
			m_matcher -> handle(req, session);
		}while(!client -> close());
	}
}
}
