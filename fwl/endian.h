#ifndef __H_ENDIAN__
#define __H_ENDIAN__ 

#include <stdint.h>
#include <byteswap.h>

#define FWL_BIG_ENDIAN 1
#define FWL_LITTLE_ENDIAN 2

namespace fwl{

template<typename T>
typename std::enable_if<sizeof(T) == sizeof(uint16_t), T>::type byteswap(T val){
	return (T)bswap_16((uint16_t)val);
}

template<typename T>
typename std::enable_if<sizeof(T) == sizeof(uint32_t), T>::type byteswap(T val){
	return (T)bswap_32((uint32_t)val);
}

template<typename T>
typename std::enable_if<sizeof(T) == sizeof(uint64_t), T>::type byteswap(T val){
	return (T)bswap_64((uint64_t)val);
}

#if BYTE_ORDER == BIG_ENDIAN
#define FWL_BYTE_ORDER FWL_BIG_ENDIAN
#else
#define FWL_BYTE_ORDER FWL_LITTLE_ENDIAN
#endif 

#if FWL_BYTE_ORDER == FWL_BIG_ENDIAN	//big endian
template<typename T>
T byteswapOnBigEndian(T t){
	return byteswap(t);
}

template<typename T>
T byteswapOnLittleEndian(T t){
	return t;
}

#else	//little endian
template<typename T>
T byteswapOnBigEndian(T t){
	return t;
}

template<typename T>
T byteswapOnLittleEndian(T t){
	return byteswap(t);
}
#endif 

}
#endif 
