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
	 * */	
	HttpRequest::ptr recvRequest();

	/**
	 * @brief sendResponse
	 * */
	int sendResponse(HttpResponse::ptr res);
};

}
}
#endif 