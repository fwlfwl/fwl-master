#include "../fwl/uri.h"
#include "../fwl/log.h"
#include "../fwl/iomanager.h"
#include <vector>
#include <memory>
#include <string>

static fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

void test_uri(const std::string & str){
	fwl::Uri::ptr uri = fwl::Uri::Create(str);
	const char * t = &("host"[0]);
	std::string hh(t, 0);
	if(!uri){
		FWL_LOG_ERROR(g_logger) << "Uri parse failed!";
		return;
	}else{
		FWL_LOG_INFO(g_logger) << "Uri::" << (uri -> toString());
		fwl::Address::ptr addr = uri -> CreateAddress();
		FWL_LOG_INFO(g_logger) << "Address info:" << (addr -> toString());
	}
}

int main(int argc, char * argv[]){
	if(2 > argc){
		FWL_LOG_WARN(g_logger) << "Usage: " << argv[0] << " [uri]";
	   return 1;	
	}
	std::string uri(argv[1]);
	fwl::IOManager iom(3);
	iom.scheduler(std::bind(test_uri, uri));
	return 0;
}
