#include <string>
#include "../fwl/http/http_parser.h"
#include "../fwl/log.h"

static fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

static const char request_data[] = "POST /LAPI/V1.0/Channels/0/Media/Video/Streams/0/Streamings HTTP/1.1\r\n"
"Host:192.168.4.44\r\n"
"Connection:keep-alive\r\n"
"Content-Length:155\r\n"
"Cookie:isAutoStartVideo=0;IPCWebLoginHandle=141786b6627867ca\r\n\r\n"
"{\"Src:\"\"test\"}\r\n";

void test_request(){
	fwl::http::HttpRequestParser::ptr parser(new fwl::http::HttpRequestParser);
	std::string tmp = request_data;
	size_t parserSize = parser -> execute(&tmp[0], tmp.size());
	FWL_LOG_INFO(g_logger) << "Parser size:" << parserSize 
		<< ",hasError:" << (0 == parser -> hasError() ? "No Error," : "Has Error,")
		<< ",ContentLength:" << (parser -> getContentLength());
	tmp.resize(tmp.size() - parserSize);
	FWL_LOG_INFO(g_logger) << std::endl 
		<< (parser -> getData() -> toString())
		<< tmp;
}

static const char response_data[] = "HTTP/1.1 200 Ok\r\n"
"Content-Length: 263\r\n"
"Content-Type: text/plain\r\n"
"Connection: close\r\n"
"X-Frame-Options: SAMEORIGIN\r\n"
"X-XSS-Protection: 1;mode=block\r\n\r\n"
"{\r\n"
"\"Response\": {\r\n"
"	\"ResponseURL\": \"/LAPI/V1.0/Channels/0/Media/Video/Streams/0/Streamings\",\r\n"
"	\"CreatedID\": -1, \r\n"
"	\"ResponseCode\": 0,\r\n"
" 	\"SubResponseCode\": 0,\r\n"
" 	\"ResponseString\": \"Succeed\",\r\n"
"	\"StatusCode\": 0,\r\n"
"	\"StatusString\": \"Succeed\",\r\n"
"	\"Data\": \"null\"\r\n"
"	}\r\n"
"}\r\n";

void test_response(){
	fwl::http::HttpResponseParser::ptr parser(new fwl::http::HttpResponseParser);
	std::string tmp = response_data;
	size_t parserSize = parser -> execute(&tmp[0], tmp.size(),false);
	FWL_LOG_INFO(g_logger) << "Parser size:" << parserSize 
		<< ",hasError:" << (0 == parser -> hasError() ? "No Error," : "Has Error,")
		<< ",ContentLength:" << (parser -> getContentLength());
	tmp.resize(tmp.size() - parserSize);
	FWL_LOG_INFO(g_logger) << std::endl 
		<< (parser -> getData() -> toString())
		<< tmp;
}
int main(int argc, char * argv[]){
	test_request();
	FWL_LOG_INFO(g_logger) << "_______";
	test_response();
	return 0;
}
