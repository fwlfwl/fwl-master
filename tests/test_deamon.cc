#include "../fwl/http/ws_server.h"
#include "../fwl/log.h"
#include "../fwl/address.h"
#include "../fwl/iomanager.h"
#include "../fwl/unit.h"
#include "../fwl/system/daemon.h"
#include <vector>
#include <memory>

static fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

void test_server(bool isMask){
	std::vector<std::pair<fwl::Address::ptr, uint32_t> > addrs;
	if(!fwl::Address::GetInterfaceAddresses(addrs, "lo")){
		FWL_LOG_ERROR(g_logger) << "GetInterfaceAddresses failed";
		return;
	}
	fwl::Address::ptr addr = (*addrs.begin()).first;
	fwl::IPAddress::ptr ipAddr = std::dynamic_pointer_cast<fwl::IPAddress>(addr);
	ipAddr -> setPort(8080);
	fwl::http::WsServer::ptr server(new fwl::http::WsServer);	
	server -> bind(addr);
	auto slt = server -> getServlet();
	slt -> addCompleteServlet("/fwl/top",[isMask](fwl::http::HttpRequest::ptr req, fwl::http::WsSession::ptr session){
				while(true){
					auto msg = session -> recvMessage();
					if(msg){
						std::string str = msg -> getData();
						//FWL_LOG_DEBUG(g_logger) << str;
						//for(int i = 0; i < (int)str.size(); ++i){
						//	FWL_LOG_DEBUG(g_logger) << i <<":" << std::hex << (int)str[i];
						//}
						if(msg -> getOpcode() == fwl::http::OPCODE::CLOSE){
							FWL_LOG_DEBUG(g_logger) << "remote close";	
							//wsClose(session.get(), fwl::http::NORMAL_CLOSE);
							session -> close();
							break;
						}
						sleep(3);
						session -> sendMessage(fwl::http::OPCODE::TEXT, true, isMask, msg -> getData() + ",back");
					}
				}
			});
	slt -> addFuzzyServlet("/fwl/*", [](fwl::http::HttpRequest::ptr req, fwl::http::WsSession::ptr session){
			});
	server -> start();
	sleep(2);
	server -> stop();
}

int main_fun(int argc, char * argv[]){
	FWL_LOG_INFO(g_logger) << "Main function start";
	fwl::IOManager iom(3);
	iom.scheduler(std::bind(test_server,0));	
	FWL_LOG_INFO(g_logger) << "Main function end";
	return 0;
}

int main(int argc, char * argv[]){
	if(argc < 2){
		FWL_LOG_ERROR(g_logger) << "Usage:" << argv[0] << " isDaemon(true/false) " << "isBackground(true/false)";
		return 1;
	}
	
	if(strcasecmp(argv[2], "true") == 0){
		int isBackground = fwl::create_background();
		if(1 == isBackground){
			return 0;	//主进程退出
		}
	}	//	子进程继续执行
	int isDaemon = false;
	if(0 == strcasecmp(argv[1], "true"))
		isDaemon = true;
	return fwl::start_daemon(argc, argv, main_fun, isDaemon);
}
