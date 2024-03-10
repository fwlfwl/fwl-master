#include "http_connection.h"
#include "http.h"
#include "http_session.h"

namespace fwl{
namespace http{
/**
 * @brief doRequest 
 * @param[in] req: http request 
 * @param[in] uri: the uri of request 
 * @param[in] uri: timeout of httpResponse 
 * */
HttpResult::ptr HttpConnection::doRequest(HttpRequest::ptr req,Uri::ptr uri, uint64_t timeout){
	//get address
	Address::ptr addr = uri -> CreateAddress();
	if(!addr){
		return std::make_shared<HttpResult>((int)RESULT::INVALID_HOST, nullptr, "Invalid host " + uri-> getHost());
	}
	//connect address
	bool rt = m_sock -> connect(addr);
	if(!rt){
		return std::make_shared<HttpResult>((int)RESULT::CONNECT_ERROR, nullptr, "Connect "+ (addr -> toString()) + " failed");
	}
	//set recv timeout
	m_sock -> setRecvTimeout(timeout);
	HttpSession::ptr session(new HttpSession(m_sock, false));
	int len = session -> sendRequest(req);
	if(0 > len){
		return std::make_shared<HttpResult>((int)RESULT::SEND_FAIL, nullptr, "Send msg failed,strerror(errno):" + std::string(strerror(errno)));
	}else if(0 == len){
		return std::make_shared<HttpResult>((int)RESULT::REMOTE_CLOSE, nullptr, "Send request close by remote address:" + addr -> toString());
	}
	auto res = session -> recvResponse();
	if(!res){
		return std::make_shared<HttpResult>((int)RESULT::RECV_FAIL, nullptr, "recv msg failed,strerror(errno):" + std::string(strerror(errno)));
	}
	return std::make_shared<HttpResult>((int)RESULT::SUCCEED,  res, "doRequest succeed");
}

/**
 * @brief doRequest 
 * @param[in] method: http request method 
 * @param[in] uri : the uri of request 
 * @param[in] headers: the headers of request 
 * @param[in] body: the body of request 
 * @param[in] timeout: timeout of http response
 * */
HttpResult::ptr HttpConnection::doRequest(HttpMethod method, Uri::ptr uri, uint64_t timeout, const std::map<std::string, std::string> & headers, const std::string & body){
	if(!uri){
		return std::make_shared<HttpResult>((int)RESULT::INVALID_URI, nullptr, "Uri is null"); 
	}
	//create HttpRequest 
	HttpRequest::ptr req(new HttpRequest);
	req -> setMethod(method);
	req -> setPath(uri -> getPath());
	req -> setQuery(uri -> getQuery());
	req -> setFragment(uri -> getFragment());
	//set headers
	//host symble
	bool hasHost = false; 
	for(auto & it : headers){
		if(0 == strcasecmp("connection", it.first.c_str())){
			if(0 == strcasecmp("keep-alive", it.second.c_str())){
				req -> setClose(false);
			}
		}else if(!hasHost && 0 == strcasecmp("host", it.first.c_str())){
			req -> setHeader("Host", it.second);
			hasHost = true;
		}else{
			req -> setHeader(it.first, it.second);
		}
	}
	//if not has host 
	if(!hasHost){
		//header add host
		req -> setHeader("Host", uri -> getHost());
	}
	req -> setBody(body);	
	return doRequest(req, uri, timeout);
}

/**
 * @brief doGet 
 * @param[in] uri : the uri of request 
 * @param[in] headers: the headers of request 
 * @param[in] body: the body of request 
 * @param[in] timeout: timeout of http response
 * */
HttpResult::ptr HttpConnection::doGet(Uri::ptr uri, uint64_t timeout, const std::map<std::string, std::string> & headers, const std::string & body){
	return doRequest(HttpMethod::GET, uri, timeout, headers, body);	
}

/**
 * @brief doGet 
 * @param[in] uri : the uri of request(string) 
 * @param[in] headers: the headers of request 
 * @param[in] body: the body of request 
 * @param[in] timeout: timeout of http response
 * */
HttpResult::ptr HttpConnection::doGet(const std::string& uri, uint64_t timeout, const std::map<std::string, std::string> & headers, const std::string & body){
	Uri::ptr uri_p = Uri::Create(uri);	
	if(!uri_p){
		return std::make_shared<HttpResult>((int)RESULT::INVALID_URI, nullptr, "Uri parse failed");
	}
	return doGet(uri_p, timeout, headers, body);
}

/**
 * @brief doPost
 * @param[in] uri : the uri of request 
 * @param[in] headers: the headers of request 
 * @param[in] body: the body of request 
 * @param[in] timeout: timeout of http response
 * */
HttpResult::ptr HttpConnection::doPost(Uri::ptr uri, uint64_t timeout, const std::map<std::string, std::string> & headers, const std::string & body){
	return doRequest(HttpMethod::POST, uri, timeout, headers, body);
}

/**
 * @brief doPost
 * @param[in] uri : the uri of request (std::string)
 * @param[in] headers: the headers of request 
 * @param[in] body: the body of request 
 * @param[in] timeout: timeout of http response
 * */
HttpResult::ptr HttpConnection::doPost(const std::string& uri, uint64_t timeout, const std::map<std::string, std::string> & headers, const std::string & body){
	Uri::ptr uri_p = Uri::Create(uri);
	if(!uri_p){
		return std::make_shared<HttpResult>((int)RESULT::INVALID_URI, nullptr, "Uri parse failed");
	}
	return doPost(uri_p, timeout, headers, body);	
}

}
}
