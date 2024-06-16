#include "../fwl/network_server.h"
#include "../fwl/address.h"
#include "../fwl/log.h"
#include "../fwl/iomanager.h"

#include <vector>
#include <memory>

fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

void test_server(){
	std::vector<std::pair<fwl::Address::ptr, uint32_t> >  addrs;
	if(!fwl::Address::GetInterfaceAddresses(addrs, "lo")){
		FWL_LOG_ERROR(g_logger) << "GetInterfaceAddresses failed";
		return;
	}
	fwl::Address::ptr addr = (*addrs.begin()).first;
	fwl::IPAddress::ptr ipAddr = std::dynamic_pointer_cast<fwl::IPAddress>(addr);
	ipAddr -> setPort(8080);
	fwl::NetworkServer::ptr server(new fwl::NetworkServer);	
	server -> bind(addr);	
	server -> start();
	sleep(2);
	server -> stop();
}


int main(int argc, char * argv[]){
	fwl::IOManager iom(2);
	iom.scheduler(test_server);
	return 0;
}
