#include "fwl/address.h"
#include "fwl/log.h"

fwl::Logger::ptr g_logger = FWL_LOG_ROOT();

void test() {
    std::vector<fwl::Address::ptr> addrs;

    FWL_LOG_INFO(g_logger) << "begin";
    //bool v = fwl::Address::Lookup(addrs, "localhost:3080");
    bool v = fwl::Address::Lookup(addrs, "www.baidu.com:http", AF_INET);
    //bool v = fwl::Address::Lookup(addrs, "www.fwl.top", AF_INET);
    FWL_LOG_INFO(g_logger) << "end";
    if(!v) {
        FWL_LOG_ERROR(g_logger) << "lookup fail";
        return;
    }

    for(size_t i = 0; i < addrs.size(); ++i) {
        FWL_LOG_INFO(g_logger) << i << " - " << addrs[i]->toString();
    }

    auto addr = fwl::Address::LookupAny("localhost:4080");
    if(addr) {
        FWL_LOG_INFO(g_logger) << *addr;
    } else {
        FWL_LOG_ERROR(g_logger) << "error";
    }
}

void test_iface() {
    std::multimap<std::string, std::pair<fwl::Address::ptr, uint32_t> > results;

    bool v = fwl::Address::GetInterfaceAddresses(results);
    if(!v) {
        FWL_LOG_ERROR(g_logger) << "GetInterfaceAddresses fail";
        return;
    }

    for(auto& i: results) {
        FWL_LOG_INFO(g_logger) << i.first << " - " << i.second.first->toString() << " - "
            << i.second.second;
    }
}

void test_ipv4() {
    //auto addr = fwl::IPAddress::Create("www.baidu.com");
    auto addr = fwl::IPAddress::Create("127.0.0.8");
    if(addr) {
        FWL_LOG_INFO(g_logger) << addr->toString();
    }
}

int main(int argc, char** argv) {
    test_ipv4();
    test_iface();
    test();
    return 0;
}
