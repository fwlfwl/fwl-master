#include "network_server.h"
#include "log.h"
#include "bytearray.h"
#include "config.h"

#include <string.h>

static fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");
static fwl::ConfigVar<uint64_t>::ptr g_recv_timeout =  fwl::Config::lookUp("g_recv_timeout", (uint64_t)60000, "getRecvTimeout");

namespace fwl{

#define STRING_TO_TYPE(str)	\
	const char * tmp  = str.c_str();	\
	if(0 == strcasecmp(tmp, "UDP")){	\
		m_type = UDP;	\
	}else if(0 == strcasecmp(tmp, "UNIX")){	\
		m_type = UNIX;	\
	}else{	\
		m_type = TCP;	\
	}

/**	
 * @brief constructor
 * @param[in] iom : IOManager
 * */
NetworkServer::NetworkServer(const std::string & name, const std::string& type, IOManager * iom, IOManager * work, int reuse):
	m_iom(iom),
	m_work(work),
	m_timeout(g_recv_timeout -> getValue()),
	m_reuse(reuse),
	m_name(name){
	m_isStop = true;
	STRING_TO_TYPE(type)
}

/**
 * @brief destructor
 * */
NetworkServer::~NetworkServer(){
	for(auto & it : m_socks){
		it -> close();
	}		
	m_socks.clear();
}

/**
 * @brief bind Address
 * */
bool NetworkServer::bind(const Address::ptr addr){
	std::vector<Address::ptr> addrs;
	std::vector<Address::ptr> fails;
	addrs.push_back(addr);
	return bind(addrs, fails);
}

/**
 * @brief bind Address(array of address)
 * */
bool NetworkServer::bind(const std::vector<Address::ptr> & addrs, std::vector<Address::ptr> & fails){
	if(TCP == m_type){
		//fails.clear();
		for(auto & addr : addrs){
			Socket::ptr sock = Socket::CreateTCP(addr);
			if(!sock -> bind(addr)){
				FWL_LOG_ERROR(g_logger) << "Bind address "
					<< (addr -> toString())
					<< " failed,errno=" << errno	
					<< ",strerror:" << strerror(errno);
				fails.push_back(addr);
				continue;	
			}
			if(!sock -> listen()){
				FWL_LOG_ERROR(g_logger) << "Listen address "
					<< (addr -> toString())
					<< " failed,errno=" << errno	
					<< ",strerror:" << strerror(errno);
				fails.push_back(addr);
				continue;					
			}
			m_socks.push_back(sock);
		}
	}
	if(!fails.empty()){
		return false;
	}
	for(auto & sock : m_socks){
		//默认设置SO_REUSEADDR和SO_REUSEPORT
		sock -> setOption(SOL_SOCKET, SO_REUSEADDR, &m_reuse);
		sock -> setOption(SOL_SOCKET, SO_REUSEPORT, &m_reuse);
		FWL_LOG_INFO(g_logger) << (sock -> toString());
	}
	return true;
}

/**
 * @brief start accept
 * */
bool NetworkServer::accept(Socket::ptr sock){
	while(!m_isStop){
		Socket::ptr client = sock -> accept();
		FWL_LOG_DEBUG(g_logger) << client -> toString();
		if(!client){	
			FWL_LOG_ERROR(g_logger) << "Accept failed, errno = "
				<< errno 
				<< ",strerror"	
				<< strerror(errno);	
		}else{
			setnonblocking(client -> getSocket());	
			client -> setRecvTimeout(m_timeout);
			m_work -> scheduler(std::bind(&NetworkServer::handlerClient, shared_from_this(), client));
		}		
	}
	return true;	
}

/**
 * @brief start server
 * */
void NetworkServer::start(){
	if(!m_isStop){
		return;
	}
	m_isStop = false; 
	for(auto & sock : m_socks){
		m_iom -> scheduler(std::bind(&NetworkServer::accept, shared_from_this(), sock));
	}
}

/**
 * @brief stop server
 * */
void NetworkServer::stop(){	
	if(m_isStop){	//if have stop, return
		return;
	}
	auto self = shared_from_this();
	for(auto & sock : m_socks){	
		m_iom -> scheduler([sock, self](){
					sock -> cancelAll();		
				});
	}	
}

/**
 * @brief get NetworkType
 * */
std::string NetworkServer::getType() const{
#undef XX 
#define XX(type)	\
	return std::string(#type);
	XX(m_type)
#undef XX
}

/**
 * @brief set NetworkType
 * */
void NetworkServer::setType(const std::string & type){
	STRING_TO_TYPE(type)
}

/**
 * @brief client handler
 * @details test echo msg
 * */
void NetworkServer::handlerClient(Socket::ptr client){
	ByteArray::ptr ba(new ByteArray);
	std::vector<iovec> iov;
	while(true){
		ba -> clear();
		ba -> getWriteBuffers(iov,(uint64_t)1024);
		int rt = client -> recv(&iov[0], iov.size());
		if(0 == rt){
			FWL_LOG_INFO(g_logger) << "Client close,"
				<< "errno = " << errno
				<<",strerror(errno):" << strerror(errno);
			break;
		}else if(0 > rt){
			FWL_LOG_INFO(g_logger) << "Msg recv error,"
				<< "errno = " << errno
				<<",strerror(errno):" << strerror(errno);
			break;
		}
		ba -> setPosition(ba -> getPosition() + rt);
		ba -> setPosition(0);
		FWL_LOG_INFO(g_logger) << "Recv msg:"
			<< std::endl
			<< (ba -> toString())
			<< (ba -> toHexString());
	}
}
#undef STRING_TO_TYPE
}
