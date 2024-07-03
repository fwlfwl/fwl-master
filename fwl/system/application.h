#ifndef __H_APPLICATION__
#define __H_APPLICATION__ 
/**
 * @brief 程序启动函数
 * */

#include <map>
#include <vector>
#include <string>
#include "../singletion.h"
#include "../network_server.h"
#include "../iomanager.h"

namespace fwl{
/**
 * @brief Application程序启动类
 * */
class Application{
public:
	//初始化函数
	bool init(int argc, char * argv[]);
	//run函数
	bool run();	
	//主运行函数 
	int main(int argc, char * argv[]);
   //fiber函数
   void runFiber();	
	//获取服务信息
	bool getServers(const std::string & servName, std::vector<NetworkServer::ptr> & servs);
	//获取所有服务信息
	void getAllServers(std::map<std::string, std::vector<NetworkServer::ptr> > &servs);
private:
	//读取服务器信息
	std::map<std::string, std::vector<NetworkServer::ptr> > m_servs;
	//主函数IoManager
	IOManager::ptr m_main_iom;
	//工作函数IoManager
	IOManager::ptr m_work_iom;
	int m_argc;
	char ** m_argv;
};

typedef Singletion<Application> appMgr;

}
#endif
