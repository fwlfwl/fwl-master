#include "fwl/http/http.h"
#include "fwl/log.h"

void test_request() {
    fwl::http::HttpRequest::ptr req(new fwl::http::HttpRequest);
    req->setHeader("host" , "www.baidu.com");
    req->setBody("hello fwl");
    req->dump(std::cout) << std::endl;
}

void test_response() {
    fwl::http::HttpResponse::ptr rsp(new fwl::http::HttpResponse);
    rsp->setHeader("X-X", "fwl");
    rsp->setBody("hello fwl");
    rsp->setStatus((fwl::http::HttpStatus)400);
    rsp->setClose(false);

    rsp->dump(std::cout) << std::endl;
}

int main(int argc, char** argv) {
    test_request();
    test_response();
    return 0;
}
