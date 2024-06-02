#include <cstdlib>
#include "application.h"
#include "env.h"
#include "daemon.h"
#include "serverConf.h"
#include "../log.h"
#include "../config.h"
#include "../http/http_server.h"
#include "../http/ws_server.h"

namespace fwl{

static Logger::ptr g_logger = FWL_LOG_NAME("system");
static const std::string DEFAULT_CONFIG_DIR("./config/");

static ConfigVar<std::vector<TcpConfig> >::ptr g_server_config = Config::lookUp("servers",
		std::vector<TcpConfig>{},"display all servers");
static ConfigVar<uint32_t>::ptr g_thread_num = Config::lookUp("system.threadNum", (uint32_t)10, "thread nums");
//初始化函数
bool Application::init(int argc, char * argv[]){
	m_argc = argc;
	m_argv = argv;
	//环境变量初始化
	EnvMgr::getInstance() -> addHelp("-d", "run as daemon");
	EnvMgr::getInstance() -> addHelp("-s", "run as terminal");
	EnvMgr::getInstance() -> addHelp("-c", "./config");
	EnvMgr::getInstance() -> addHelp("-h", "print helps");
	bool is_print = false;
	if(!EnvMgr::getInstance() -> init(argc, argv) || EnvMgr::getInstance() -> has("h")){
		//异常退出，则设置打印
		is_print = true;
	}
	//返回运行结果	
	if(is_print){
		EnvMgr::getInstance() -> printHelps();
		return false;
	}
	//配置文件加载
	Config::LoadFromDir(EnvMgr::getInstance() -> get("c", DEFAULT_CONFIG_DIR));	
	return true;
}

//run函数
bool Application::run(){
	bool is_daemon = EnvMgr::getInstance() -> has("d");
	return 0 == start_daemon(m_argc, m_argv,std::bind(&Application::main, this, std::placeholders::_1, std::placeholders::_2), is_daemon);	
}

//主运行函数 
int Application::main(int argc, char * argv[]){
	//再次加载文件
	Config::LoadFromDir(EnvMgr::getInstance() -> get("c", DEFAULT_CONFIG_DIR));
	//运行函数
	m_main_iom.reset(new IOManager(g_thread_num -> getValue(), true, "main"));
	m_main_iom -> scheduler(std::bind(&Application::runFiber,this));	//运行函数 
	m_main_iom -> stop();
	return 0;
}	

//运行主函数
void Application::runFiber(){
	//信号初始化
	//signal_init();
	//获取所有服务信息 
	auto g_servers = g_server_config -> getValue();
	FWL_LOG_DEBUG(g_logger) << "g_servers size:" << g_servers.size();
	for(auto & g_server : g_servers){
		std::vector<Address::ptr> g_addrs;	//记录所有解析地址信息
		std::vector<Address::ptr> fails;	//记录监听失败地址信息
		for(auto & addr_str : g_server.m_addrs){
			//解析字符串
			auto pos = addr_str.find(':');
			if(pos == std::string::npos)
				continue;
			uint16_t port = std::atoi(addr_str.substr(pos + 1).c_str());		
			Address::ptr addr = IPAddress::Create(addr_str.substr(0,pos).c_str(), port);
			if(addr){	//地址处
				g_addrs.push_back(addr); 
				continue;
			}	
			//通过网口解析
			std::vector<std::pair<Address::ptr, uint32_t> > result;
			if(Address::GetInterfaceAddresses(result, addr_str.substr(0,pos))){
				for(auto & it : result){
					auto a = std::dynamic_pointer_cast<IPAddress>(it.first);
					a -> setPort(port);
					g_addrs.push_back(it.first);
				}
				continue;
			}
			//通过域名解析
			auto a = IPAddress::LookupAnyIPAddress(addr_str.substr(0, pos));
			if(addr){
				a -> setPort(port);
				g_addrs.push_back(a);
			}
		}
		//绑定servers 
		NetworkServer::ptr server;
		auto typeName = g_server.m_service_type;
		if(0 == strcasecmp(typeName.c_str(),"http")){
			server.reset(new http::HttpServer(g_server.m_keepalive));
			m_servs["http"].push_back(server);
		}else if(0 == strcasecmp(typeName.c_str(), "websocket")){
			server.reset(new http::WsServer);
			m_servs["websocket"].push_back(server);
		}else{
				FWL_LOG_ERROR(g_logger) << "Create server error!";
				break;
		}
		server -> bind(g_addrs,fails);
	}
	//开始监听并运行
	for(auto & serv : m_servs){
		for(auto & it : serv.second){
			it -> start();
		}
	}
}

//获取服务信息
bool Application::getServers(const std::string & servName, std::vector<NetworkServer::ptr> & servs){
	auto it = m_servs.find(servName);
	if(it == m_servs.end())
		return false;
	servs = m_servs[servName];
	return true;	
}

//获取所有服务信息
void Application::getAllServers(std::map<std::string, std::vector<NetworkServer::ptr> > & servs){
	servs = m_servs;
}

}
