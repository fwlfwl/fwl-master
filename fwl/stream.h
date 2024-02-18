/**
 * @file stream.h
 * @brief stream package
 * @author fwl
 * @email 1822363660@qq.com
 * @date 2024-2-17
**/
#ifndef __H_STREAM__
#define __H_STREAM__ 

#include <memory>
#include "bytearray.h"

namespace fwl{
/**
 * @brief stream class
 * */	
class Stream{
public:
	/**
	 * @brief destructor function
	 * */
	virtual ~Stream(){}
	
	/**
	 * @brief read content(recv msg)
	 * @param[in] buf: the content of buffer
	 * @param[in] len: the length of buffer
	 * */
	virtual int read(void * buf, size_t len) = 0;
	
	/**
	 * @brief read content(recv msg)
	 * @param[in] ba: the content of ByteArray
	 * @param[in] len: the length of buffer
	 * */
	virtual int read(ByteArray::ptr ba, size_t len) = 0;
	
	/**
	 * @brief read content(recv msg,read full return )
	 * @param[in] buf: the content of buffer
	 * @param[in] len: the length of buffer
	 * */
	virtual int readFix(void * buf, size_t len);
	
	/**
	 * @brief read content(recv msg,read full return)
	 * @param[in] ba: the content of ByteArray
	 * @param[in] len: the length of buffer
	 * */
	virtual int readFix(ByteArray::ptr ba, size_t len);
	
	/**
	 * @brief write content(send msg)
	 * @param[in] buf: the content of buffer
	 * @param[in] len: the length of buffer
	 * */
	virtual int write(const void * buf, size_t len) = 0;
	
	/**
	 * @brief write content(send msg)
	 * @param[in] buf: the content of buffer
	 * @param[in] len: the length of buffer
	 * */
	virtual int write(ByteArray::ptr ba, size_t len) = 0;

	/**
	 * @brief write content(send msg,write full return)
	 * @param[in] buf: the content of buffer
	 * @param[in] len: the length of buffer
	 * */
	virtual int writeFix(const void * buf, size_t len);
	
	/**
	 * @brief write content(send msg,write full return)
	 * @param[in] buf: the content of buffer
	 * @param[in] len: the length of buffer
	 * */
	virtual int writeFix(ByteArray::ptr ba, size_t len);
};
}

#endif
