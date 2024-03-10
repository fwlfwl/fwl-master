
#include "../fwl/http/http_session.h"
#include "../fwl/address.h"
#include "../fwl/log.h"
#include "../fwl/iomanager.h"
#include "../fwl/socket.h"
#include "../fwl/http/http.h"
#include "../fwl/macro.h"

#include <vector>
#include <memory>

fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

void test_session(const char * path){
	ASSERT(path);
	fwl::Address::ptr addr = fwl::Address::LookupAny("www.sylar.top:80");
	//fwl::IPAddress::ptr ipAddr = std::dynamic_pointer_cast<fwl::IPAddress>(addr);
	fwl::Socket::ptr sock = fwl::Socket::CreateTCP(addr);
	bool rt = sock -> connect(addr);
	if(!rt){
		FWL_LOG_ERROR(g_logger) << "Connect to " << (addr -> toString()) << "failed!";
		return;
	}
	fwl::http::HttpRequest::ptr req(new fwl::http::HttpRequest);
	req -> setMethod(fwl::http::CharsToHttpMethod("GET"));
	req -> setPath(path);
	req -> setHeader("Host","www.sylar.top");
	fwl::http::HttpSession::ptr session(new fwl::http::HttpSession(sock, true));
	session -> sendRequest(req);
	fwl::http::HttpResponse::ptr res =  session -> recvResponse();
	if(!req){	
		FWL_LOG_ERROR(g_logger) << "response parser error!";
		return;
	}	
	FWL_LOG_INFO(g_logger) << "Response:" << std::endl << (res -> toString());
}


int main(int argc, char * argv[]){
	if(2 > argc){
		FWL_LOG_WARN(g_logger) << "Usage: " << argv[0] << " [path]";
		return 1;
	}
	fwl::IOManager iom(2);
	iom.scheduler(std::bind(test_session, argv[1]));
	//FWL_LOG_INFO(g_logger) << (0 | MSG_PEEK);
	return 0;
}
