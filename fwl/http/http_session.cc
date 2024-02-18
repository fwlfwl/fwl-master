#include "http_session.h"
#include "http_parser.h"
#include "../log.h"

static fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

namespace fwl{
namespace http{
/**
 * @brief construcor
 * @param[in] sock 
 * @param[in] owner:the ownership of sock
 * */
HttpSession::HttpSession(Socket::ptr sock, bool owner):
	SockStream(sock, owner){}

/**
 * @brief recvRequest
 * */	
HttpRequest::ptr HttpSession::recvRequest(){
	HttpRequestParser::ptr parser(new HttpRequestParser);
	uint64_t bufferSize = HttpRequestParser::GetHttpRequestBufferSize();
	std::shared_ptr<char> buffer(new char[bufferSize], [](char * ptr){
			delete[] ptr;
			});	
	char * data = buffer.get();
	size_t offset = 0;
	do{
		//FWL_LOG_DEBUG(g_logger) << m_sock -> toString(); 
		int len = read(data + offset , bufferSize - offset);
		//FWL_LOG_DEBUG(g_logger) << "len:" << len; 
		if(len <= 0){
			return nullptr;
		}	
		len += offset;
		size_t nparser = parser -> execute(data, len);
		if(parser -> hasError()){
			return nullptr;
		}		
		offset = len - nparser;
		if(offset == (size_t)bufferSize){
			return nullptr;
		}
		if(parser -> isFinished()){
			break;
		}
	}while(true);
	//read body 
	uint64_t contentLen = parser -> getContentLength();
	int readBodyLen = readFix(data, contentLen);
	if(readBodyLen < 0){
		return nullptr;
	}
	parser -> getData() -> setBody(data);	
	parser -> getData() -> init();
	return parser -> getData();
}

/**
 * @brief sendResponse
 * */
int HttpSession::sendResponse(HttpResponse::ptr res){
	if(!res){
		return -1;
	}
	std::string resStr = res -> toString();
	return writeFix(resStr.c_str(), resStr.size());
}

}
}
