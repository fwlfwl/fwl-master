/**
 * @file sockStream.h
 * @brief sockStream package
 * @author fwl
 * @email 1822363660@qq.com
 * @date 2024-2-17
**/
#ifndef __H_SOCKSTREAM__
#define __H_SOCKSTREAM__

#include <memory>
#include "socket.h"
#include "stream.h"
#include "bytearray.h"

namespace fwl{
/**
 * @brief sockStream class
 * */
class SockStream : public Stream{
public:
	/**
	 * @brief construcor
	 * @param[in] sock 
	 * @param[in] owner:the ownership of sock
	 * */
	SockStream(Socket::ptr sock, bool owner, int64_t timeout = 0);

	/**
	 * @brief destructor 
	 * */	
	~SockStream();
	/**
	 * @brief get socket connect state
	 * */
	bool isConnected(){	return m_sock && m_sock -> isConnected();	}

	/**
	 * @breif
	 * */
	Socket::ptr getSocket() const {	return m_sock;	}	

	/**
	 * @brief read content(recv msg)
	 * @param[in] buf: the content of buffer
	 * @param[in] len: the length of buffer
	 * */
	int read(void * buf, size_t len) override;
	
	/**
	 * @brief read content(recv msg)
	 * @param[in] ba: the content of ByteArray
	 * @param[in] len: the length of buffer
	 * */
	int read(ByteArray::ptr ba, size_t len) override;
	
	/**
	 * @brief write content(send msg)
	 * @param[in] buf: the content of buffer
	 * @param[in] len: the length of buffer
	 * */
	int write(const void * buf, size_t len) override;
	
	/**
	 * @brief write content(send msg)
	 * @param[in] buf: the content of buffer
	 * @param[in] len: the length of buffer
	 * */
	int write(ByteArray::ptr ba, size_t len) override;

	/**
	 * @brief close sock
	 * */
	void close();
protected:
	//default recv timeout 
	int64_t m_recvTimeout;
	//connect sock
	Socket::ptr m_sock;
	//ownership of sock 
	bool m_owner;
};

}

#endif
