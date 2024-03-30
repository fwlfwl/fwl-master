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
	std::string body;
	if(offset >= contentLen){
		body.append(data, contentLen);
	}else{
		body.append(data, offset);
		memset(data, '\0', bufferSize);
		int needMsg = contentLen - offset;
		while(0 < needMsg){
			int nread = read(data, (uint64_t)needMsg > bufferSize ? bufferSize : needMsg);	//read more msg
			if(0 >= nread){
				FWL_LOG_ERROR(g_logger) << "Can not read enough msg,body content is incomplete!";
				return nullptr;
			}
			int writeLen = nread > needMsg ? needMsg : nread;
			body.append(data, writeLen);
			needMsg -= writeLen;
		}
	}
	if(!body.empty())
		parser -> getData() -> setBody(body);
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
