#include "../fwl/http/http_connection.h"
#include "../fwl/address.h"
#include "../fwl/log.h"
#include "../fwl/iomanager.h"
#include "../fwl/socket.h"
#include "../fwl/http/http.h"
#include "../fwl/macro.h"

#include <vector>
#include <memory>

fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

void test_http_session(const char * path){
	ASSERT(path);
	fwl::Address::ptr addr = fwl::Address::LookupAny("www.sylar.top:80");
	//fwl::IPAddress::ptr ipAddr = std::dynamic_pointer_cast<fwl::IPAddress>(addr);
	fwl::Socket::ptr sock = fwl::Socket::CreateTCP(addr);
	fwl::http::HttpConnection::ptr conn(new fwl::http::HttpConnection(sock, true));	
	if(!conn){
		FWL_LOG_ERROR(g_logger) << "HttpConnection create fail";
		return;
	}
	auto result = conn -> doGet(path, 0);
	if(result -> m_response){
		FWL_LOG_INFO(g_logger) << "Response msg:" << std::endl << (result -> m_response -> toString());
	}else{
		FWL_LOG_ERROR(g_logger) << "Response msg recv error, " << (result -> m_error);
	}
}


int main(int argc, char * argv[]){
	if(2 > argc){
		FWL_LOG_WARN(g_logger) << "Usage: " << argv[0] << " [path]";
		return 1;
	}
	fwl::IOManager iom(2);
	iom.scheduler(std::bind(test_http_session, argv[1]));
	//FWL_LOG_INFO(g_logger) << (0 | MSG_PEEK);
	return 0;
}
