#include "http_connection.h"
#include "http_session.h"
#include "../socket.h"
#include "../log.h"
#include "../unit.h"

static const int CHUNK_DONE = 1;
static fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

namespace fwl{
namespace http{

/**
 * @brief bool is valid
 * */
bool HttpConnection::isValid() const{
	return m_sock -> isValid() && m_sock -> isConnected();
}

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
	int len = sendRequest(req);
	if(0 > len){
		return std::make_shared<HttpResult>((int)RESULT::SEND_FAIL, nullptr, "Send msg failed,strerror(errno):" + std::string(strerror(errno)));
	}else if(0 == len){
		return std::make_shared<HttpResult>((int)RESULT::REMOTE_CLOSE, nullptr, "Send request close by remote address:" + addr -> toString());
	}
	auto res = recvResponse();
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
	if(!hasHost && !uri -> getHost().empty()){
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

/**
 * @brief sendRequest 
 * @details As client 
 * */
int HttpConnection::sendRequest(HttpRequest::ptr req){
	if(!req){
		return -1;
	}
	std::string reqStr = req -> toString();
	return writeFix(reqStr.c_str(),reqStr.size()); 
}

/**
 * @brief recvResponse
 * @details As client 
 * */
HttpResponse::ptr HttpConnection::recvResponse(){	
	uint64_t response_max_size = HttpResponseParser::GetHttpResponseBufferSize() - 1;
	std::shared_ptr<char> buffer(new char[response_max_size], [](char * ptr){
			delete[] ptr;
			});
	char * data = buffer.get();
	HttpResponseParser::ptr parser(new HttpResponseParser);
	size_t offset = 0;
	do{
		int len = read(data + offset, response_max_size - offset);
#define RECV_CHECK(len)	\
		if(0 > len){	\
			if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR){	\
				continue;	\
			}	\
			close();	\
			return nullptr;	\
		}else if(0 == len){	\
			close();	\
			return nullptr;	\
		}	
		do{
			RECV_CHECK(len);
		}while(0);
		offset += len;
		data[offset] = '\0';
		/*if(offset == response_max_size){
			//recv msg too long
			return nullptr;
		}*/
		
		size_t nparse = parser -> execute(data, offset, false);
		offset -= nparse;		
		if(parser -> hasError()){
			close();
			return nullptr;
		}
	}while(!parser -> isFinished());
	//FWL_LOG_DEBUG(g_logger) << (parser -> getData() -> toString());
	//record body 
	std::string body;
	//httpclient_parser
	auto & clientparser = parser -> getParser();
	if(clientparser.chunked){	//response is chunk
		do{
			do{	//read chunkLen
				int nread = 0;
				if(0 == offset){
					//no data, need read more msg
					nread = read(data, response_max_size);
					do{
						RECV_CHECK(nread);
					}while(0);
				}
				offset += nread;
				data[offset] = '\0';
				//FWL_LOG_DEBUG(g_logger) << data << std::endl << offset;
				size_t nparse = parser -> execute(data, offset, true);
				offset -= nparse;
				if(parser -> hasError()){
					close();
					return nullptr;
				}
			}while(!parser -> isFinished());
			if(CHUNK_DONE == clientparser.chunks_done){
				//chunks done
				break;
			}
			//FWL_LOG_DEBUG(g_logger) << std::hex << clientparser.content_len;
			if(0 > clientparser.content_len){
				FWL_LOG_ERROR(g_logger) << "Error:chunk size is " << clientparser.content_len;
				return nullptr;
			}
			size_t chunkLen = clientparser.content_len + 2;
			if(chunkLen > offset){ //data is no enough,need read more msg,end by CRTL 
				body.append(data, offset);
				int left = chunkLen - offset;
			   	while(0 < left){
					int nread = read(data, (int)response_max_size > left ? left : response_max_size);
					do{
						//can not read complete msg
						RECV_CHECK(nread);
					}while(0);
					int writeLen = nread > left ? left : nread;
					//FWL_LOG_DEBUG(g_logger) << strlen(data) << "," << writeLen;
					body.append(data, writeLen);
					//reset offset
					memmove(data, data + writeLen, response_max_size - writeLen);
					offset = nread - writeLen;
					left -= writeLen;
				}	//one chunk read complete	
				body.resize(body.size() - 2);
			}else{
				body.append(data, clientparser.content_len);
				offset -= chunkLen;
				memmove(data, data + chunkLen, offset);
			}	
		}while(true);
	}else{
		uint64_t contentLen = parser -> getContentLength();
		if(offset >= contentLen){
			body.append(data, contentLen);
		}else{
			body.append(data, offset);
			memset(data, '\0', response_max_size);
			int needMsg = contentLen - offset;
			while(0 < needMsg){
				int nread = read(data, (uint64_t)needMsg < response_max_size ? needMsg : response_max_size);	//read more msg
				do{
					RECV_CHECK(nread);
#undef RECV_CHECK
				}while(0);
				int writeLen = nread > needMsg ? needMsg : nread;
				body.append(data, writeLen);
				needMsg -= writeLen;
				//no need memmove, is end
			}
		}
	}
	auto response = parser -> getData();
	if(!body.empty()){
		parser -> getData() -> setBody(body);
		for(auto & i : response -> getHeaders()){
			//if Connection:Close,then close sock 
			if(!response -> isWebsocket() && 0 == strcasecmp(i.first.c_str(), "connection") && 0 == strcasecmp(i.second.c_str(), "close")){
				close();
			}
		}
	}
	return parser -> getData();
}

/**
 * @brief create http connection pool
 * */
HttpConnectionPool::ptr HttpConnectionPool::Create(const std::string & uri, uint32_t maxSize, uint64_t maxAliveTime){
	Uri::ptr uriObj = Uri::Create(uri);
	if(!uriObj){
		FWL_LOG_DEBUG(g_logger) << "Uri format error";
		return nullptr;
	}
	return std::make_shared<HttpConnectionPool>(uriObj -> getHost(), uriObj -> getPort(), 0 == strcasecmp((uriObj -> getScheme()).c_str(), "https"), maxSize, maxAliveTime);
}

/**
 * @brief constructor 
 * */
HttpConnectionPool::HttpConnectionPool(const std::string & host, uint32_t port, bool is_https, uint32_t maxSize, uint64_t maxAliveTime):
	m_isHttps(is_https),
	m_host(host),
	m_port(port ? port : (is_https ? 443 : 80)),
	m_maxSize(maxSize),
	m_maxAliveTime(maxAliveTime){}

/**
 * @brief get connection 
 * */
HttpConnection::ptr HttpConnectionPool::getConnection(){
	uint64_t now_t = fwl::GetTimeMs();
	HttpConnection::ptr conn;
	MutexType::Lock lock(m_lock);
	while(!m_conns.empty()){
		conn = *m_conns.begin();
		m_conns.pop_front();
		if(!conn -> isValid() || (m_maxAliveTime + conn -> m_createTime) > now_t){
			continue;
		}
		break;
	}
	lock.unlock();
	if(!conn && m_maxSize > m_conns.size()){
		IPAddress::ptr addr = Address::LookupAnyIPAddress(m_host);
		if(!addr){
			FWL_LOG_ERROR(g_logger) << "Can not parse host";
			return nullptr;
		}
		addr -> setPort(m_port);
		Socket::ptr sock = m_isHttps ? SSLSocket::CreateTCP(addr) : Socket::CreateTCP(addr);
		if(!sock){
			FWL_LOG_ERROR(g_logger) << "Can not create sock";
			return nullptr;
		}
		bool rt = sock -> connect(addr);
		if(!rt){
			FWL_LOG_ERROR(g_logger) << "Can not connect remote addr" << (addr -> toString());
			return nullptr;
		}
		conn.reset(new HttpConnection(sock, true));
	}
	return conn;
}

/**
 * @brief reuse  
 * */
void HttpConnectionPool::reuse(HttpConnection::ptr conn){
	if(conn && conn -> isConnected() && m_maxAliveTime + conn -> m_createTime >= GetTimeMs()){
		MutexType::Lock lock(m_lock);
		m_conns.push_back(conn);
		++m_total;
	}
}

/**
 * @brief doRequest 
 * @param[in] req: http request 
 * @param[in] uri: the uri of request 
 * @param[in] uri: timeout of httpResponse 
 * */
HttpResult::ptr HttpConnectionPool::doRequest(HttpRequest::ptr req, uint64_t timeout){
	auto conn = getConnection();
	if(!conn){
		return std::make_shared<HttpResult>((int)RESULT::POOL_GET_CONNECTION_FAIL, nullptr, "pool get connection failed");
	}
	auto sock = conn -> m_sock;
   	sock -> setRecvTimeout(timeout);
	int len = conn -> sendRequest(req);
	if(0 > len){
		return std::make_shared<HttpResult>((int)RESULT::SEND_FAIL, nullptr, "Send msg failed,strerror(errno):" + std::string(strerror(errno)));
	}else if(0 == len){
		return std::make_shared<HttpResult>((int)RESULT::REMOTE_CLOSE, nullptr, "Send request close by remote address:" + sock -> getRemoteAddress() -> toString());
	}
	//FWL_LOG_DEBUG(g_logger) << "Start recv response";
	auto res = conn -> recvResponse();
	//FWL_LOG_DEBUG(g_logger) << "End recv response";
	if(!res){
		return std::make_shared<HttpResult>((int)RESULT::RECV_FAIL, nullptr, "recv msg failed,strerror(errno):" + std::string(strerror(errno)));
	}
	reuse(conn);	
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
HttpResult::ptr HttpConnectionPool::doRequest(HttpMethod method, Uri::ptr uri, uint64_t timeout, const std::map<std::string, std::string> & headers, const std::string & body){
	if(!uri){
		return std::make_shared<HttpResult>((int)RESULT::INVALID_URI, nullptr, "Uri is null"); 
	}
	HttpRequest::ptr req(new HttpRequest);
	req -> setMethod(method);
	req -> setPath(uri -> getPath());
	req -> setQuery(uri -> getQuery());
	req -> setFragment(uri -> getFragment());
	bool hasHost = false;
	for(auto & header : headers){
		if(0 == strcasecmp("connection", header.first.c_str()) && 0 == strcasecmp("keep-alive", header.second.c_str())){
			req -> setClose(false);
		}else if(0 == strcasecmp("host", header.first.c_str())){	
			req -> setHeader("Host", header.second);
			hasHost = true;
		}else{
			req -> setHeader(header.first, header.second);
		}
	}
	if(!hasHost){
		req -> setHeader("Host", m_host);
	}
	req -> setBody(body);
	return doRequest(req, timeout);
}

/**
 * @brief doGet 
 * @param[in] uri : the uri of request 
 * @param[in] headers: the headers of request 
 * @param[in] body: the body of request 
 * @param[in] timeout: timeout of http response
 * */
HttpResult::ptr HttpConnectionPool::doGet(Uri::ptr uri, uint64_t timeout, const std::map<std::string, std::string> & headers, const std::string & body){
	return doRequest(HttpMethod::GET, uri, timeout, headers, body);
}

/**
 * @brief doGet 
 * @param[in] uri : the uri of request(string) 
 * @param[in] headers: the headers of request 
 * @param[in] body: the body of request 
 * @param[in] timeout: timeout of http response
 * */
HttpResult::ptr HttpConnectionPool::doGet(const std::string& uri, uint64_t timeout, const std::map<std::string, std::string> & headers, const std::string & body){
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
HttpResult::ptr HttpConnectionPool::doPost(Uri::ptr uri, uint64_t timeout, const std::map<std::string, std::string> & headers, const std::string & body){
	return doRequest(HttpMethod::POST, uri, timeout, headers, body);
}

/**
 * @brief doPost
 * @param[in] uri : the uri of request (std::string)
 * @param[in] headers: the headers of request 
 * @param[in] body: the body of request 
 * @param[in] timeout: timeout of http response
 * */
HttpResult::ptr HttpConnectionPool::doPost(const std::string& uri, uint64_t timeout, const std::map<std::string, std::string> & headers, const std::string & body){
	Uri::ptr uri_p = Uri::Create(uri);
	if(!uri_p){
		return std::make_shared<HttpResult>((int)RESULT::INVALID_URI, nullptr, "Uri parse failed");
	}
	return doPost(uri_p, timeout, headers, body);
}

/*doPut, doPost, ...*/
}
}
