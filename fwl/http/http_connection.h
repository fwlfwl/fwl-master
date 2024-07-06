/**
 * @file http_connection.h
 * @brief http connector 
 * @author fwl
 * @email 1822363660@qq.com
 * @date 2024-3-3
**/

#include <list>
#include <memory>
#include <atomic>
#include "http.h"
#include "../sock_stream.h"
#include "../uri.h"
#include "../unit.h"

namespace fwl{
namespace http{

//返回结果
enum RESULT{
	INVALID_INPUT = 1,	//invalid input 
	INVALID_HOST,
	CONNECT_ERROR,
	INVALID_URI,
	SEND_FAIL,
	RECV_FAIL,
	REMOTE_CLOSE,	//remote address close socket
	SUCCEED,
	POOL_GET_CONNECTION_FAIL
};

/**
 * @brief http connection result
 * */
class HttpResult{
public:
	typedef std::shared_ptr<HttpResult> ptr;
	HttpResult(int result, HttpResponse::ptr res, const std::string & error):
		m_response(res),
		m_result(result),
		m_error(error){}
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
	friend class HttpConnectionPool;
	typedef std::shared_ptr<HttpConnection> ptr;
	/**
	 * @brief constructor
	 * */
	HttpConnection(Socket::ptr sock, bool owner = false, uint64_t createTime = GetTimeMs()):
		SockStream(sock, owner),
		m_createTime(createTime){}

	/**
	 * @brief destructor 
	 * */
	~HttpConnection(){}

	/**
	 * @brief bool is valid
	 * */
	bool isValid() const;

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
	HttpResult::ptr doRequest(HttpMethod method, Uri::ptr uri, uint64_t timeout, const std::map<std::string, std::string> & headers, const std::string & body);

	/**
	 * @brief doGet 
	 * @param[in] uri : the uri of request 
	 * @param[in] headers: the headers of request 
	 * @param[in] body: the body of request 
	 * @param[in] timeout: timeout of http response
	 * */
	HttpResult::ptr doGet(Uri::ptr uri, uint64_t timeout, const std::map<std::string, std::string> & headers ={}, const std::string & body = "");
	
	/**
	 * @brief doGet 
	 * @param[in] uri : the uri of request(string) 
	 * @param[in] headers: the headers of request 
	 * @param[in] body: the body of request 
	 * @param[in] timeout: timeout of http response
	 * */
	HttpResult::ptr doGet(const std::string& uri, uint64_t timeout, const std::map<std::string, std::string> & headers = {}, const std::string & body = "");
	
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

	/*doPut, doPost, ...*/
private:
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
private:
	uint64_t m_createTime;
};

/**
 * @brief connection pool 
 * */
class HttpConnectionPool{
public:
	typedef std::shared_ptr<HttpConnectionPool> ptr;
	typedef Mutex MutexType;	
	/**
	 * @brief create http connection pool
	 * */
	static HttpConnectionPool::ptr Create(const std::string & uri, uint32_t maxSize, uint64_t maxAliveTime);

	/**
	 * @brief constructor 
	 * */
	HttpConnectionPool(const std::string & host, uint32_t port, bool is_https, uint32_t maxSize, uint64_t maxAliveTime);
	
	/**
	 * @brief get connection 
	 * */
	HttpConnection::ptr getConnection();
	
	/**
	 * @brief doRequest 
	 * @param[in] req: http request 
	 * @param[in] uri: the uri of request 
	 * @param[in] uri: timeout of httpResponse 
	 * */
	HttpResult::ptr doRequest(HttpRequest::ptr req, uint64_t timeout);
	
	/**
	 * @brief doRequest 
	 * @param[in] method: http request method 
	 * @param[in] uri : the uri of request 
	 * @param[in] headers: the headers of request 
	 * @param[in] body: the body of request 
	 * @param[in] timeout: timeout of http response
	 * */
	HttpResult::ptr doRequest(HttpMethod method, Uri::ptr uri, uint64_t timeout, const std::map<std::string, std::string> & headers, const std::string & body);

	/**
	 * @brief doGet 
	 * @param[in] uri : the uri of request 
	 * @param[in] headers: the headers of request 
	 * @param[in] body: the body of request 
	 * @param[in] timeout: timeout of http response
	 * */
	HttpResult::ptr doGet(Uri::ptr uri, uint64_t timeout, const std::map<std::string, std::string> & headers ={}, const std::string & body = "");
	
	/**
	 * @brief doGet 
	 * @param[in] uri : the uri of request(string) 
	 * @param[in] headers: the headers of request 
	 * @param[in] body: the body of request 
	 * @param[in] timeout: timeout of http response
	 * */
	HttpResult::ptr doGet(const std::string& uri, uint64_t timeout, const std::map<std::string, std::string> & headers = {}, const std::string & body = "");
	
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

	/*doPut, doPost, ...*/

private:
	/**
 	* @brief reuse  
 	* */
	void reuse(HttpConnection::ptr conn);

private:
	//is https 
	bool m_isHttps = false;
	//remote host 
	std::string m_host;
	//remote port 
	uint32_t m_port;
	//max Connection number 
	uint32_t m_maxSize;
	//max alive time 
	uint64_t m_maxAliveTime;
	//total num 
	std::atomic<uint32_t> m_total;
	//lock 
	MutexType m_lock;	
	//Connection pools 
	std::list<HttpConnection::ptr> m_conns;
};

}
}

