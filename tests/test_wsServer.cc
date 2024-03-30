#include "../fwl/http/ws_server.h"
#include "../fwl/log.h"
#include "../fwl/address.h"
#include "../fwl/iomanager.h"
#include <vector>
#include <memory>

static fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

void test_server(){
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
	slt -> addCompleteServlet("/fwl/top",[](fwl::http::HttpRequest::ptr req, fwl::http::WsSession::ptr session){
				auto msg = session -> recvMessage();
				session -> sendMessage(fwl::http::OPCODE::TEXT, true, false, msg -> getData());
			});
	slt -> addFuzzyServlet("/fwl/*", [](fwl::http::HttpRequest::ptr req, fwl::http::WsSession::ptr session){
			});
	server -> start();
	sleep(2);
	server -> stop();
}

int main(int argc, char * argv[]){
	fwl::IOManager iom(3);
	iom.scheduler(test_server);
}
