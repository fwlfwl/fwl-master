#ifndef __H_APPLICATION__
#define __H_APPLICATION__ 
/**
 * @brief 程序启动函数
 * */

#include <vector>
#include "../network_server.h"
#include "../iomanager.h"

/**
 * @brief Application程序启动类
 * */
class Application{
public:
	//初始化函数
	bool init();
	//主运行函数
	bool run();	
	//获取服务信息
	bool getServers(std::string, std::vector<NetworkType::ptr> servs);
	//获取所有服务信息
	void getAllServers(std::map<std::string, std::vector<NetworkServer::ptr> > servs);
private:
	//读取服务器信息
	std::map<std::string, std::vector<NetworkServer::ptr> > m_servs;
	//主函数IoManager
	IOManager::ptr m_main_iom;
}

#endif
