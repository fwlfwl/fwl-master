#include "ws_session.h"
#include "../config.h"
#include "../log.h"
#include "../endian.h"
#include "../unit.h"

#define BASE_SIZE 1024

static fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");
static fwl::ConfigVar<uint32_t>::ptr g_websocket_message_max_size = fwl::Config::lookUp("websocket_message_max_size", "websocket message max size", (uint32_t)(1024 * 1024 * 32));

namespace fwl{
namespace http{

#define WEBSOCKET_STATE_TO_STRING(state) #state

/**
 * @brief sendMessage
 * */ 
bool WsSession::sendMessage(uint8_t opcode, bool fin, bool mask, const std::string & msg, const std::string & extend_data){
	return wsSendMessage(this, opcode, fin, mask, msg, extend_data);
}

/**
 * @brief recvMessage
 * */ 
WsFrameMessage::ptr WsSession::recvMessage(){
	return wsRecvMessage(this);	
}

/**
 * @brief handlerServer
 * */
WsSession::Result WsSession::handleshake(){
	auto req = recvRequest();
	//rfc6455 4.1
	do{
		if(!req){
			wsClose(this, PROTOCOL_ERROR);
			return std::make_shared<SharkResult>(nullptr, (int)SHARK_STATE::FMT_ERROR);
		}
		//version check 
		if(0x10 == req -> getVersion() )	{
			FWL_LOG_ERROR(g_logger) << "Http version lower than 1.1";
			wsClose(this, PROTOCOL_ERROR);
			return std::make_shared<SharkResult>(nullptr, (int)SHARK_STATE::HTTP_VERSION_ERROR);
		}
		//method check
		if(HttpMethod::GET != req -> getMethod()){
			FWL_LOG_ERROR(g_logger) << "Request not be Get";
			wsClose(this, PROTOCOL_ERROR);
			return std::make_shared<SharkResult>(nullptr, (int)SHARK_STATE::METHOD_ERROR);
		}
		//Connection check
		if(0 != strcasecmp(&(req -> getHeader("connection"))[0], "Upgrade")){
			FWL_LOG_ERROR(g_logger) << "Connection is not Upgrade";
			wsClose(this, PROTOCOL_ERROR);
			return std::make_shared<SharkResult>(nullptr, (int)SHARK_STATE::HEADER_ERROR);
		}		
		//upgrad check 
		if(0 != strcasecmp(&(req -> getHeader("Upgrade"))[0], "websocket")){
			FWL_LOG_ERROR(g_logger) << "Upgrade is not websocket";
			wsClose(this, PROTOCOL_ERROR);
			return std::make_shared<SharkResult>(nullptr, (int)SHARK_STATE::HEADER_ERROR);
		}
		//sec-websocket-key check
		std::string key = req -> getHeader("Sec-WebSocket-Key");
		if(key.empty()){
			FWL_LOG_ERROR(g_logger) << "Sec-WebSocket-Key is empty";
			wsClose(this, PROTOCOL_ERROR);
			return std::make_shared<SharkResult>(nullptr, (int)SHARK_STATE::KEY_ERROR);
		}
		key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
		unsigned char out[20];
		fwl::sha1(key, out);
		std::string out_key = fwl::base64En(out, 20);
		
		//create response  
		HttpResponse::ptr res(new HttpResponse(0x11, false));
		res -> setHeader("Connection", "Upgrade");
		res -> setHeader("Upgrade", "websocket");
		res -> setHeader("Sec-WebSocket-Accept",out_key);
		res -> setWebsocket(true);
		//Version is no Supported
		if(0 != strcasecmp(&(req -> getHeader("Sec-WebSocket-Version"))[0], "13")){
			res -> setStatus(HttpStatus::BAD_REQUEST);
			res -> setReason("Version is invalid");
			res -> setHeader("Sec-WebSocket_Key", "13");
			sendResponse(res);
			wsClose(this, PROTOCOL_ERROR);
			return std::make_shared<SharkResult>(nullptr, (int)PROTOCOL_ERROR);
		}	
		//success response 
		
		res -> setStatus(HttpStatus::SWITCHING_PROTOCOLS);
		res -> setReason("Switching Protocols");
		
		sendResponse(res);
	}while(0);
	return std::make_shared<SharkResult>(req, (int)SHARK_STATE::NORMAL);
}

/**
 * @brief Client-to-Server masking
 * */
#define MASKING(isMask, mask, data, beginIndex, endIndex)	\
	if(isMask){	\
		for(size_t i = beginIndex; i < endIndex; ++i){	\
			data[i] ^= mask[i % 4]; 	\
		}	\
	}

#define WRITE(buf, len)	\
		do{	\
			int left_v = len;	\
			do{	\
				int writen = stream -> write(buf, left_v);	\
				if(0 > writen){	\
					if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR){	\
						continue;	\
					}	\
					stream -> close();	\
					return false;	\
				}	\
				left_v -=writen;	\
			}while(0 < left_v);	\
		}while(0);

/**
 * @brief sendMssage
 * @param[in] opcode : the interpretation of "paylaod data"
 * @param[in] fin : isFin
 * @param[in] mask: mask-key
 * @param[in] msg : send message(Application Data; if mask, data will be masking after sendMessage)
 * @param[in] extend_data: send message(Extension Data; if mask, data will be masking after sendMssage)
 * @param[in] mask_cb : mask generation function  
 * */ 
bool wsSendMessage(SockStream * stream, uint8_t opcode, bool fin, bool mask, const std::string & msg_in, const std::string & extend_data_in){
	//create WsFrameHead
	WsFrameHead head;
	head.fin = fin;
	head.rsv1 = false;
	head.rsv2 = false;
	head.rsv3 = false;
	head.mask = mask;
	head.opcode = opcode;
	uint64_t payloadLen = extend_data_in.size() + msg_in.size();
	if(payloadLen < 126){
		head.payload_len = payloadLen;
	}else if(payloadLen < 0xFF){
		head.payload_len = 126;
	}else{
		head.payload_len = 127;
	}
	
	//write websocket frame head
	ByteArray::ptr ba(new ByteArray(BASE_SIZE));
	ba -> write((void *)&head, sizeof(head));	
	//WRITE(&head, sizeof(head));
	
	//write extended payload length	
	if(head.payload_len == 126){
		uint16_t len = byteswapOnLittleEndian((uint16_t)payloadLen);
		ba -> write((void *)&len, sizeof(len));	
		//WRITE((void*)&len, sizeof(len))
	}else if(head.payload_len == 127){
		uint64_t len = byteswapOnLittleEndian(payloadLen);
		ba -> write((void *)&len, sizeof(len));	
		//WRITE((void*)&len, sizeof(len))
	}

	//write head mask
	char mask_v[4];
	if(head.mask){
		uint32_t randValue = rand();
		memcpy(mask_v, (void*)&randValue, 4);
		ba -> write((void *)mask_v, 4);
		//WRITE((void *)&mask_v[0], 4)		
	}
	std::string msg = msg_in;
	std::string extend_data = extend_data_in;
	MASKING(head.mask, mask_v, extend_data, 0, extend_data.size());
	ba -> write((void *)&extend_data[0], extend_data.size());
	MASKING(head.mask, mask_v, msg, extend_data.size(), payloadLen);
	ba -> write((void *)&msg[0], msg.size());
	//WRITE((void *)&extend_data[0], extend_data.size())
	//WRITE((void *)&msg[0], msg.size())
	ba ->setPosition(0);
	//FWL_LOG_DEBUG(g_logger) << "Msg length:" << ba -> getSize() << ",send msg:" << (ba -> toString());
	WRITE(ba, ba -> getSize())
	return true;
}
#undef WRITE

#define RECV_CHECK(len)	\
		if(0 > len){	\
			if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR){	\
				continue;	\
			}	\
			stream -> close();	\
			return nullptr;	\
		}else if(0 == len){	\
			stream -> close();	\
			return nullptr;	\
		}	
/**
 * @brief recvMessage
 * */
WsFrameMessage::ptr wsRecvMessage(SockStream * stream){
	std::string data;
	//read num 
	uint64_t readSum;
	WsFrameHead head;
	int opcode = 0;
	do{
		readSum = 0;
		int readn = 0, readHeader = 0;
		int headSize = sizeof(WsFrameHead);
		//recv WsFrameHead 
		unsigned char headC[headSize];
		do{
			readn = stream -> read(&headC[readHeader], headSize - readHeader);
			RECV_CHECK(readn);
			readHeader += readn;
		}while(readHeader < headSize);
		readSum += headSize;
		memcpy((void * )&head, (void *)headC, sizeof(head));
		FWL_LOG_DEBUG(g_logger) << (head.toString()) << std::hex << head.opcode;
		//head check
		//rsv1,rsv2,rsv3 must be 0 
		if(head.rsv1 || head.rsv2 || head.rsv3){
			wsClose(stream, int(PROTOCOL_ERROR));
			return nullptr;
		}
	
		if(head.opcode == PING){
			if(!wsPong(stream)){
				FWL_LOG_ERROR(g_logger) << "PING error";
				return nullptr;
			}	
		}else if(head.opcode == PONG){
		}else if(head.opcode == TEXT ||
				head.opcode == BINARY ||
				head.opcode == CONTINUE ||
				head.opcode == CLOSE){
			if(CONTINUE != head.opcode){
				opcode = head.opcode;
				//FWL_LOG_DEBUG(g_logger) << opcode;
			}
			//Get payload length(if has extended length)
			uint64_t payloadLen;
			if(head.payload_len == 126){
				uint64_t len; 
				readn = stream -> read(&len, 2);
				RECV_CHECK(readn)
				readSum += readn;
				payloadLen = byteswapOnLittleEndian(len);
			}else if(head.payload_len == 127){
				uint64_t len;
				readn = stream -> read(&len, 8);
				RECV_CHECK(readn)
				readSum += readn;
				payloadLen = byteswapOnLittleEndian(len);
			}else{
				payloadLen = (uint64_t)head.payload_len;
			}
			//Get mask 
			char mask[4];
			if(head.mask){
				readn = stream -> read(&mask, sizeof(mask));
				RECV_CHECK(readn);
				readSum += sizeof(mask);
			}
			//计算已有的数据大小
			uint64_t cur_len = data.size();
			//payloadLen > g_websocket_message_max_size ??
			if(g_websocket_message_max_size -> getValue() < readSum + payloadLen + cur_len){
				FWL_LOG_WARN(g_logger) << "Websocket length > "
					<< g_websocket_message_max_size 
					<< "(" << (readSum + payloadLen + cur_len) << ")";
				return nullptr;
			}
			//Get payload data 
			uint64_t readS = 0;
			data.resize(cur_len + payloadLen);
			do{
				readn = stream -> read(&data[cur_len + readS], payloadLen - readS);
				RECV_CHECK(readn);
				readSum += readn;
				readS += readn;
			}while(readS < payloadLen);
			//mask data(if mask)
			MASKING(head.mask, mask, data, cur_len, data.size());
			if(head.fin){
				return std::make_shared<WsFrameMessage>(opcode, data);
			}
		}else{
			FWL_LOG_ERROR(g_logger) << "Invalid opcode";
			break;
		}
	}while(true);
	return nullptr;
}

/**
 * @brief ping
 * */
bool wsPing(SockStream * stream){
	return wsSendMessage(stream, OPCODE::PING, true);
}

/***
 * @brief PONG(stream::Ptr stream);
 */
bool wsPong(SockStream * stream){
	return wsSendMessage(stream, OPCODE::PONG, true);
}

/**
 * @brief Close
 * */
bool wsClose(SockStream * stream, int state){
	return wsSendMessage(stream, OPCODE::CLOSE, true, false, std::string(WEBSOCKET_STATE_TO_STRING(state)));
}

#undef RECV_CHEK 
#undef MASKING
}
}
