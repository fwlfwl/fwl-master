#include "http_server.h"
#include "../log.h"

fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

namespace fwl{
namespace http{
	/**
	 * client handler
	 * */
	void HttpServer::handlerClient(Socket::ptr client){
		HttpSession::ptr session(new HttpSession(client, true));		
		bool close = true;
		do{
			HttpRequest::ptr req = session -> recvRequest();
			if(!req){
				FWL_LOG_ERROR(g_logger) << "Sock connect error,errno:"
					<< errno << ",strerror(errno):" << strerror(errno);
				return;
			}
			close = m_isClose || req -> isClose();
			HttpResponse::ptr res(new HttpResponse);
			res -> setClose(close);
			m_matcher -> handle(req, res, session);
			session -> sendResponse(res);
			//sleep(10);
		}while(!close);
	}
}
}
