/**
 * @file network_server.h
 * @brief network_server encapsulation
 * @auth fwl
 * @email 1822363660@qq.com 
 * @data 2024-02-11
 * */
#ifndef __H_NETWORKSERVER__
#define __H_NETWORKSERVER__

#include <memory>
#include <vector>

#include "iomanager.h"
#include "socket.h"
#include "address.h"

/**
 * @brief socket type
 * */
enum NetworkType {
	TCP = 0,
	UDP,
	UNIX
};

namespace fwl{
class NetworkServer : public std::enable_shared_from_this<NetworkServer>{
public:
	typedef std::shared_ptr<NetworkServer> ptr;
	/**
	 * @brief constructor
	 * @param[in] iom : IOManager
	 * */
	NetworkServer(const std::string &name = "", const std::string& type = "TCP", IOManager * iom = fwl::IOManager::GetThis(), IOManager * work = fwl::IOManager::GetThis());

	/**
	 * @brief destructor
	 * */
	virtual ~NetworkServer();

	/**
	 * @brief bind Address
	 * */
	bool bind(const Address::ptr addr);

	/**
	 * @brief bind Address(array of address)
	 * */
	bool bind(const std::vector<Address::ptr> & addrs, std::vector<Address::ptr> & fails);

	/**
	 * @brief start accept 
	 * */
	bool accept(const Socket::ptr sock);

	/**
	 * @brief start server
	 * */
	void start();

	/**
	 * @brief stop server
	 * */
	void stop();

	/**
	 * @brief get timeout
	 * */
	uint64_t getTimeout() const {	return m_timeout;	}

	/**
	 * @brief set timeout
	 * */
	void setTimeout(uint64_t v) {	m_timeout = v;	}

	/**
	 * @brief get isStop
	 * */
	bool getIsStop() const {	return m_isStop;	}

	/**
	 * @brief get NetworkType
	 * */
	std::string getType() const;

	/**
	 * @brief set NetworkType
	 * */
	void setType(const std::string & type);

	/**
	 * @brief set server name
	 * */
	void setName(const std::string & name){	m_name = name;	}
protected:
	/**
	 * client handler
	 * */
	virtual void handlerClient(Socket::ptr client);
private:
	//IO Manager
	IOManager * m_iom;
	//work Manager 
	IOManager * m_work;
	//vector of socket
	std::vector<Socket::ptr> m_socks;
	//is stop 
	bool m_isStop;
	//recv timeout 
	uint64_t m_timeout;
	//network type(TCP,UDP,UNIX)
	NetworkType m_type;
	//server name
	std::string m_name;	
};

}

#endif
