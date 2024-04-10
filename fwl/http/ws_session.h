/**
 * @file websocket.h
 * @brief websocket package
 * @author fwl
 * @email 1822363660@qq.com
 * @date 2024-3-20
**/

#ifndef __H_WS_SESSION__
#define __H_WS_SESSION__

#include <memory>
#include <functional>
#include <utility>
#include "http_session.h"

namespace fwl{
namespace http{
/*	websocket data frame
  0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-------+-+-------------+-------------------------------+
 |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
 |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
 |N|V|V|V|       |S|             |   (if payload len==126/127)   |
 | |1|2|3|       |K|             |                               |
 +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
 |     Extended payload length continued, if payload len == 127  |
 + - - - - - - - - - - - - - - - +-------------------------------+
 |                               |Masking-key, if MASK set to 1  |
 +-------------------------------+-------------------------------+
 | Masking-key (continued)       |          Payload Data         |
 +-------------------------------- - - - - - - - - - - - - - - - +
 :                     Payload Data continued ...                :
 + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
 |                     Payload Data continued ...                |
 +---------------------------------------------------------------+
*/

/**
 * @brief websocket opcode
 * */
enum OPCODE{
	CONTINUE = 0x0,	//continuation frame
	TEXT = 0x1,	//text frame
	BINARY = 0x2,	//binary frames
	/*0x3-0x7:reserverd for further non-control frames*/
	CLOSE = 0x8, //connection close 
	PING = 0x9,	//ping 
	PONG = 0xA //pong 
	/*0xB-0xF:reserver for further control frames*/
};

/**
 * @brief websocket data frame part
 * */
struct WsFrameHead{
	uint8_t opcode:4;
	bool rsv3:1;
	bool rsv2:1;
	bool rsv1:1;
	bool fin:1;
	uint8_t payload_len:7;
	bool mask:1;

	std::string toString(){
		std::stringstream ss;
		ss << "[fin:"
			<< fin 
			<<",resv1:"
			<< rsv1 
			<<",resv2:"
			<< rsv2
			<<",rsv3:"
			<< rsv3 
			<<",opcode:"
			<<(unsigned char)opcode 
			<< ",mask:"
			<< mask 
			<<",payload_len:"
			<< (unsigned char )payload_len	
			<<"]";
		return ss.str();
	}
};

/**
 * @brief websocket data frame message
 * */
class WsFrameMessage{
public:
	typedef std::shared_ptr<WsFrameMessage> ptr;

	WsFrameMessage(int opcode, const std::string & data):
		m_opcode(opcode),
		m_data(data){}

	/**
	 * @brief getOpcode
	 * */
	int getOpcode() const{	return m_opcode;	}
	
	/**
	 * @breif setOPcode
	 * */
	void setOpcode(int opcode) {	m_opcode = opcode; }

	/**
	 * @brief getData
	 * */
	std::string getData() const  {	return m_data;	}

	/**
	 * @brief setData
	 * */
	void setData(const std::string & data) {	m_data = data;	}
private:
	int m_opcode;
	std::string m_data;
};

enum CLOSE_STATE{
	NORMAL_CLOSE = 1000,
	SERVER_ERROR = 1001,
	PROTOCOL_ERROR = 1002,
	DATA_ERROR = 1003,
	FUTURE_STATUS = 1004,	//reserved; define in futrue
	NO_STATUS = 1005, //reserved; not set in Close control frame 
	CLOSE_ABNORMALLY = 1006, //reserved; not set in Close control frame
	MESSAGE_TYPE_ERROR = 1007,
	GENERIC_CODE = 1008,
	MESSAGE_TOOBIG = 1009,
	NO_EXTENSION = 1010, //used in client; expect the server to negotiate one or more extension 
	UNEXPECTED_ERROR = 1011,
	TLS_ERROR = 1015 //reserverd; not set in Close control frame 
};

class SharkResult{
public:
	typedef std::shared_ptr<SharkResult> ptr;
	
	SharkResult(HttpRequest::ptr ptr, int state):
		m_request(ptr),
		m_state(state){}

	HttpRequest::ptr m_request;
	int m_state;
};

/**
 * @brief websocket server
 * */
class WsSession : public HttpSession{
public:
	enum class SHARK_STATE{
		NORMAL = 0,
		FMT_ERROR,
		HTTP_VERSION_ERROR,	
		METHOD_ERROR,
		HEADER_ERROR,
		URI_ERROR,
		KEY_ERROR
	};

	typedef std::shared_ptr<WsSession> ptr;
	typedef SharkResult::ptr Result;
	
	/**
	 * @brief construcor
	 * */
	WsSession(Socket::ptr sock,bool isOwner = true):
		HttpSession(sock, isOwner){}

	/**
	 * @brief sendMessage
	 * */ 
	bool sendMessage(uint8_t opcode, bool fin,bool mask, const std::string & msg = "",const std::string & extend_data = "");

	/**
	 * @brief recvMessage
	 * */ 
	WsFrameMessage::ptr recvMessage();

	/**
	 * @brief handshake
	 * */
	Result handleshake();

};

/**
 * @brief sendMssage
 * @param[in] stream  
 * @param[in] opcode : the interpretation of "paylaod data"
 * @param[in] fin : isFin
 * @param[in] mask: mask-key
 * @param[in] msg : send message(Application Data; if mask, data will be masking after sendMessage)
 * @param[in] extend_data: send message(Extension Data; if mask, data will be asking after sendMssage)
 * @param[in] mask_cb : mask generation function  
 * */ 
bool wsSendMessage(SockStream * stream, uint8_t opcode, bool fin,bool mask = false, const std::string & msg = "",const std::string & extend_data = "");

/**
 * @brief recvMessage
 * */
WsFrameMessage::ptr wsRecvMessage(SockStream * stream);

/**
 * @brief ping
 * */
bool wsPing(SockStream * stream);

/***
 * @brief PONG(SockStream::ptr stream);
 */
bool wsPong(SockStream * stream);

/**
 * @brief Close
 * */
bool wsClose(SockStream *, int state);

}
}

#endif
