#include "stream.h"
#include "log.h"

static fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");
namespace fwl{

/**
 * @brief read content(recv msg,read full return )
 * @param[in] buf: the content of buffer
 * @param[in] len: the length of buffer
 * */
int Stream::readFix(void * buf, size_t len){
	size_t offset = 0;
	int64_t left = len;
	while(left > 0){
		int64_t readLen = read((char *)buf + offset, left);
		if(readLen < 0){
			return readLen;
		}
		offset += readLen;
		left -= readLen;
	}
	return len;
}

/**
 * @brief read content(recv msg,read full return)
 * @param[in] ba: the content of ByteArray
 * @param[in] len: the length of buffer
 * */
int Stream::readFix(ByteArray::ptr ba, size_t len){
	size_t left = len;
	while(len > 0){
		int64_t readLen = read(ba, left);
		if(readLen < 0){
			return readLen;
		}
		left -= readLen;
	}
	return len;
}

/**
 * @brief write content(send msg,write full return)
 * @param[in] buf: the content of buffer
 * @param[in] len: the length of buffer
 * */
int Stream::writeFix(const void * buf, size_t len){
	size_t offset = 0;
	int64_t left = len;
	while(left > 0){
		int64_t writeLen = write(buf, left);
		if(writeLen < 0){
			return writeLen;
		}
		offset += writeLen;
		left -= writeLen;
	}
	return len;
}

/**
 * @brief write content(send msg,write full return)
 * @param[in] buf: the content of buffer
 * @param[in] len: the length of buffer
 * */
int Stream::writeFix(ByteArray::ptr ba, size_t len){
	int64_t left = len;
	while(left > 0){
		int64_t writeLen = write(ba, left);
		if(writeLen < 0){
			return writeLen;
		}
		left -= writeLen;
	}
	return len;
}

}
