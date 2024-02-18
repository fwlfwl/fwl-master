#include <sys/socket.h>
#include "sock_stream.h"
#include "log.h"

static fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

namespace fwl{

/**
 * @brief construcor
 * @param[in] sock 
 * @param[in] owner:the ownership of sock
 * */
SockStream::SockStream(Socket::ptr sock, bool owner):
	m_sock(sock),
	m_owner(owner){}

/**
 * @brief destructor  
 * */	
SockStream::~SockStream(){
	//if owner is true,close sock in destruction
	if(m_owner && m_sock){
		m_sock -> close();
	}
}

/**
 * @brief read content(recv msg)
 * @param[in] buf: the content of buffer
 * @param[in] len: the length of buffer
 * */
int SockStream::read(void * buf, size_t len){
	if(!isConnected()){
		return -1;
	}
	//FWL_LOG_DEBUG(g_logger) << m_sock -> toString();
	return m_sock -> recv(buf, len);
}

/**
 * @brief read content(recv msg)
 * @param[in] ba: the content of ByteArray
 * @param[in] len: the length of buffer
 * */
int SockStream::read(ByteArray::ptr ba, size_t len){
	if(!isConnected()){
		return -1;
	}
	std::vector<iovec> iovs;
	ba -> getWriteBuffers(iovs, len);
	int rt = m_sock -> recv(&iovs[0], len);	
	if(rt > 0){
		ba -> setPosition(ba -> getPosition() + rt);
	}
	return rt;
}

/**
 * @brief write content(send msg)
 * @param[in] buf: the content of buffer
 * @param[in] len: the length of buffer
 * */
int SockStream::write(const void * buf, size_t len){
	if(!isConnected()){
		return -1;
	}
	return m_sock -> send(buf, len);
}

/**
 * @brief write content(send msg)
 * @param[in] buf: the content of buffer
 * @param[in] len: the length of buffer
 * */
int SockStream::write(ByteArray::ptr ba, size_t len){
	if(!isConnected()){
		return -1;
	}
	std::vector<iovec> iovs;
	ba -> getReadBuffers(iovs, len);
	int rt = m_sock -> send(&iovs[0], len);
	if(rt > 0){
		ba -> setPosition(ba -> getPosition() + rt); 	
	}
	return rt;
}
}
