#include "http_session.h"
#include "http_parser.h"
#include "../log.h"

#define CHUNK_DONE 1

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
			int nread = read(data, bufferSize);	//read more msg
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

/**
 * @brief sendRequest 
 * @details As client 
 * */
int HttpSession::sendRequest(HttpRequest::ptr req){
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
HttpResponse::ptr HttpSession::recvResponse(){	
	uint64_t response_max_size = HttpResponseParser::GetHttpResponseBufferSize() - 1;
	std::shared_ptr<char> buffer(new char[response_max_size], [](char * ptr){
			delete[] ptr;
			});
	char * data = buffer.get();
	HttpResponseParser::ptr parser(new HttpResponseParser);
	size_t offset = 0;
	do{
		int len = read(data + offset, response_max_size - offset);
		if(len <= 0){
			return nullptr;
		}
		offset += len;
		data[offset] = '\0';
		/*if(offset == response_max_size){
			//recv msg too long
			return nullptr;
		}*/
		
		size_t nparse = parser -> execute(data, offset, false);
		offset -= nparse;		
		if(parser -> hasError()){
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
					if(0 >= nread){
						return nullptr;
					}
				}
				offset += nread;
				data[offset] = '\0';
				size_t nparse = parser -> execute(data, offset, true);
				offset -= nparse;
				if(parser -> hasError()){
					return nullptr;
				}
			}while(!parser -> isFinished());
			if(CHUNK_DONE == clientparser.chunks_done){
				//chunks done
				break;
			}
			FWL_LOG_DEBUG(g_logger) << std::hex << clientparser.content_len;
			if(0 > clientparser.content_len){
				FWL_LOG_ERROR(g_logger) << "Error:chunk size is " << clientparser.content_len;
				return nullptr;
			}
			size_t chunkLen = clientparser.content_len + 2;
			if(chunkLen > offset){ //data is no enough,need read more msg,end by CRTL 
				body.append(data, offset);
				int left = chunkLen - offset;
			   	while(0 < left){
					int nread = read(data, response_max_size);
					if(0 >= nread){
						//can not read complete msg
						FWL_LOG_ERROR(g_logger) << "Can not read enough msg, chunk is incomplete!";
						return nullptr;
					}
					int writeLen = nread > left ? left : nread;
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
				int nread = read(data, response_max_size);	//read more msg
				if(0 >= nread){
					FWL_LOG_ERROR(g_logger) << "Can not read enough msg,body content is incomplete!";
					return nullptr;
				}
				int writeLen = nread > needMsg ? needMsg : nread;
				body.append(data, writeLen);
				needMsg -= writeLen;
				//no need memmove, is end
			}
		}
	}
	if(!body.empty())
		parser -> getData() -> setBody(body);
	return parser -> getData();
}
}
}
