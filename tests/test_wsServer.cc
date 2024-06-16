#include "../fwl/http/ws_server.h"
#include "../fwl/log.h"
#include "../fwl/address.h"
#include "../fwl/iomanager.h"
#include "../fwl/unit.h"
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
						FWL_LOG_DEBUG(g_logger) << str;
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

int main(int argc, char * argv[]){
	if(argc < 2){
		FWL_LOG_ERROR(g_logger) << "Usage:" << argv[0] << " isMask(true/false)";
		return 1;
	}
	bool isMask = false;
	if(strcasecmp(argv[1], "true") == 0){
		isMask = true;
	}
	fwl::IOManager iom(3);
	iom.scheduler(std::bind(test_server, isMask));
	//unsigned char sha1_out[20];
   	//fwl::sha1("dGhlIHNhbXBsZSBub25jZQ==258EAFA5-E914-47DA-95CA-C5AB0DC85B11", sha1_out);
	//std::string b64_out = fwl::base64En(sha1_out, 20);
	//for(int i = 0; i < 20; ++i){
	//	std::cout << std::hex << sha1_out[i] << " ";
	//}
	//std::cout << std::endl << b64_out;
}
