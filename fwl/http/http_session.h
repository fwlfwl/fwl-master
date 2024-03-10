/**
 * @file http_session.h
 * @brief http session(recv request , send response)
 * @author fwl
 * @email 1822363660@qq.com
 * @date 2024-2-17
**/
#ifndef __H_HTTP_SESSION__
#define __H_HTTP_SESSION__ 

#include <memory>
#include "../sock_stream.h"
#include "http_parser.h"
#include "http.h"

namespace fwl{
namespace http{

class HttpSession : public SockStream{
public:
	typedef std::shared_ptr<HttpSession> ptr;
	/**
	 * @brief construcor
	 * @param[in] sock 
	 * @param[in] owner:the ownership of sock
	 * */
	HttpSession(fwl::Socket::ptr sock, bool owner);
	
	/**
	 * @brief recvRequest
	 * @details As server  
	 * */	
	HttpRequest::ptr recvRequest();

	/**
	 * @brief sendResponse 
	 * @details As server 
	 * */
	int sendResponse(HttpResponse::ptr res);

	/**
	 * @brief sendRequest 
	 * @details As client 
	 * */
	int sendRequest(HttpRequest::ptr req);

	/**
	 * @brief recvResponse
	 * @details As client 
	 * */
	HttpResponse::ptr recvResponse();
};

}
}
#endif 
