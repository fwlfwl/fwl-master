/**
 * @file http_connection.h
 * @brief http connector 
 * @author fwl
 * @email 1822363660@qq.com
 * @date 2024-3-3
**/

#include <memory>
#include "http.h"
#include "../sock_stream.h"
#include "../uri.h"

namespace fwl{
namespace http{

enum Result{

};

/**
 * @brief http connection result
 * */
class HttpResult{
public:
	typedef std::shared_ptr<HttpResult> ptr;
	HttpResult(int result, HttpResponse::ptr res, const std::string & error):
		m_result(result),
		m_response(res),
		m_error(error){}
private:
	//response msg
	HttpResponse::ptr m_response;
	//recv server result 
	int m_result;
	//error
	std::string m_error;
};
	
	
/**
 * @brief httpConnection(http client)
 * */
class HttpConnection : public SockStream{
public:
	typedef std::shared_ptr<HttpConnection> ptr;

	/**
	 * @brief constructor
	 * */
	HttpConnection(Socket::ptr sock, bool owner)
		SockStream(sock, owner){}

	/**
	 * @brief destructor 
	 * */
	~HttpConnection(){}
	
	/**
	 * @brief doRequest 
	 * @param[in] req: http request 
	 * @param[in] uri: the uri of request 
	 * @param[in] uri: timeout of httpResponse 
	 * */
	HttpResult::ptr doRequest(HttpRequest::ptr req,Uri::ptr uri, uint64_t timeout);
	
	/**
	 * @brief doRequest 
	 * @param[in] method: http request method 
	 * @param[in] uri : the uri of request 
	 * @param[in] headers: the headers of request 
	 * @param[in] body: the body of request 
	 * @param[in] timeout: timeout of http response
	 * */
	HttpResult::ptr doRequest(HttpMethod method, Uri::ptr uri, Uri::ptr uri, uint64_t timeout, const std::map<std::string, std::string> & headers, const std::string & body);

	/**
	 * @brief doGet 
	 * @param[in] uri : the uri of request 
	 * @param[in] headers: the headers of request 
	 * @param[in] body: the body of request 
	 * @param[in] timeout: timeout of http response
	 * */
	HttpResult::ptr doGet(HttpMethod method, Uri::ptr uri, Uri::ptr uri, uint64_t timeout, const std::map<std::string, std::string> & headers ={}, const std::string & body = "");
	
	/**
	 * @brief doGet 
	 * @param[in] uri : the uri of request(string) 
	 * @param[in] headers: the headers of request 
	 * @param[in] body: the body of request 
	 * @param[in] timeout: timeout of http response
	 * */
	HttpResult::ptr doGet(const std::string& uri, Uri::ptr uri,uint64_t timeout, const std::map<std::string, std::string> & headers = {}, const std::string & body = "");
	
	/**
	 * @brief doPost
	 * @param[in] uri : the uri of request 
	 * @param[in] headers: the headers of request 
	 * @param[in] body: the body of request 
	 * @param[in] timeout: timeout of http response
	 * */
	HttpResult::ptr doPost(Uri::ptr uri, uint64_t timeout, const std::map<std::string, std::string> & headers, const std::string & body);
	
	/**
	 * @brief doPost
	 * @param[in] uri : the uri of request (std::string)
	 * @param[in] headers: the headers of request 
	 * @param[in] body: the body of request 
	 * @param[in] timeout: timeout of http response
	 * */
	HttpResult::ptr doPost(const std::string& uri, uint64_t timeout, const std::map<std::string, std::string> & headers, const std::string & body);

	/**
	 * @brief send request 
	 * */
	int sendReuqst(HttpRequest::ptr request);

	/**
	 * @brief recv response 
	 * */
	HttpResponse::ptr recvResponse();
private:
};

}

