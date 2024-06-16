#include "../fwl/http/http_connection.h"
#include "../fwl/address.h"
#include "../fwl/log.h"
#include "../fwl/iomanager.h"
#include "../fwl/socket.h"
#include "../fwl/http/http.h"
#include "../fwl/macro.h"
#include "../fwl/unit.h"

//#include <b64/encode.h>
#include <vector>
#include <memory>

fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

void test_http_connection(const char * host, const char * path){
	ASSERT(path);
	fwl::Address::ptr addr = fwl::Address::LookupAny(host);
	//fwl::IPAddress::ptr ipAddr = std::dynamic_pointer_cast<fwl::IPAddress>(addr);
	if(!addr){
		FWL_LOG_ERROR(g_logger) << "Look up address fail";
		return;
	}
	fwl::Socket::ptr sock = fwl::Socket::CreateTCP(addr);
	if(!sock){
		FWL_LOG_ERROR(g_logger) << "Host is invalid";
		return;
	}
	fwl::http::HttpConnection::ptr conn(new fwl::http::HttpConnection(sock, true));	
	if(!conn){
		FWL_LOG_ERROR(g_logger) << "HttpConnection create fail";
		return;
	}
	auto result = conn -> doGet(path, 0);
	if(result -> m_response){
		FWL_LOG_INFO(g_logger) << "Response msg:"  << (result -> m_error);
	}else{
		FWL_LOG_ERROR(g_logger) << "Response msg recv error, " << (result -> m_error);
	}
}

void test_httpConnectionPool(const char * host, const char * uri, const char * method, const char * isClose){
	fwl::http::HttpConnectionPool::ptr pool = fwl::http::HttpConnectionPool::Create(host, 80, 5, 30000);
	if(!pool){
		FWL_LOG_ERROR(g_logger) << "Http Connection pool create failed";
		return;
	}
	fwl::http::HttpResult::ptr res;
	std::map<std::string, std::string> headers;
	headers["Host"] = host;
	if(0 == strcasecmp("false", isClose)){
		headers["Connection"] = "keep-alive";
	}	
	for(int i = 0; i < 3; ++i){
		//sleep(12);
		if(0 == strcasecmp("get", method)){
			res = pool -> doGet(uri, 5000, headers);
		}else{
			res = pool -> doPost(uri, 5000, headers, "{test}");
		}
		if(res && res -> m_response){
			std::ofstream ofs;
			std::string file = std::string("response" + std::to_string(i) + ".html");
			bool rt = fwl::FSUnit::OpenForWrite(ofs, file);
			if(rt){
				ofs << (res -> m_response -> getBody());
				ofs.close();
			}
			FWL_LOG_INFO(g_logger) << "Response msg:" << (res -> m_error);
		}else{
			FWL_LOG_ERROR(g_logger) << "Response msg recv error, " << (res -> m_error);
		}
	}
}

int main(int argc, char * argv[]){
	if(5 > argc){
		FWL_LOG_WARN(g_logger) << "Usage: " << argv[0] << " [method] [host] [uri] [is close]";
		return 1;
	}
	fwl::IOManager iom(2);
	iom.scheduler(std::bind(test_httpConnectionPool, argv[2], argv[3], argv[1],argv[4]));
	iom.scheduler(std::bind(test_httpConnectionPool, argv[2], argv[3], argv[1],argv[4]));
	//test_httpConnectionPool(argv[2], argv[3], argv[1],argv[4]);
	return 0;
}
