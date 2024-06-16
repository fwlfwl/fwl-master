#include "fwl/socket.h"
#include "fwl/iomanager.h"
#include "fwl/unit.h"

static fwl::Logger::ptr g_looger = FWL_LOG_ROOT();

void test_socket() {
    //std::vector<sylar::Address::ptr> addrs;
    //sylar::Address::Lookup(addrs, "www.baidu.com", AF_INET);
    //sylar::IPAddress::ptr addr;
    //for(auto& i : addrs) {
    //    SYLAR_LOG_INFO(g_looger) << i->toString();
    //    addr = std::dynamic_pointer_cast<sylar::IPAddress>(i);
    //    if(addr) {
    //        break;
    //    }
    //}
    fwl::IPAddress::ptr addr = fwl::Address::LookupAnyIPAddress("www.baidu.com");
    if(addr) {
        FWL_LOG_INFO(g_looger) << "get address: " << addr->toString();
    } else {
        FWL_LOG_ERROR(g_looger) << "get address fail";
        return;
    }

    fwl::Socket::ptr sock = fwl::Socket::CreateTCP(addr);
    addr->setPort(80);
    FWL_LOG_INFO(g_looger) << "addr=" << addr->toString();
    if(!sock->connect(addr)) {
        FWL_LOG_ERROR(g_looger) << "connect " << addr->toString() << " fail";
        return;
    } else {
        FWL_LOG_INFO(g_looger) << "connect " << addr->toString() << " connected";
    }

    const char buff[] = "GET / HTTP/1.0\r\n\r\n";
    int rt = sock->send(buff, sizeof(buff));
    if(rt <= 0) {
        FWL_LOG_INFO(g_looger) << "send fail rt=" << rt;
        return;
    }

    std::string buffs;
    buffs.resize(4096);
    rt = sock->recv(&buffs[0], buffs.size());

    if(rt <= 0) {
        FWL_LOG_INFO(g_looger) << "recv fail rt=" << rt;
        return;
    }

    buffs.resize(rt);
    FWL_LOG_INFO(g_looger) << buffs;
}

void test2() {
    fwl::IPAddress::ptr addr = fwl::Address::LookupAnyIPAddress("www.baidu.com:80");
    if(addr) {
        FWL_LOG_INFO(g_looger) << "get address: " << addr->toString();
    } else {
        FWL_LOG_ERROR(g_looger) << "get address fail";
        return;
    }

    fwl::Socket::ptr sock = fwl::Socket::CreateTCP(std::dynamic_pointer_cast<fwl::Address>(addr));
    if(!sock->connect(addr)) {
        FWL_LOG_ERROR(g_looger) << "connect " << addr->toString() << " fail";
        return;
    } else {
        FWL_LOG_INFO(g_looger) << "connect " << addr->toString() << " connected";
    }

    uint64_t ts = fwl::GetTimeUs();
    for(size_t i = 0; i < 100000000ul; ++i) {
        if(int err = sock->getError()) {
            FWL_LOG_INFO(g_looger) << "err=" << err << " errstr=" << strerror(err);
            break;
        }

        //struct tcp_info tcp_info;
        //if(!sock->getOption(IPPROTO_TCP, TCP_INFO, tcp_info)) {
        //    SYLAR_LOG_INFO(g_looger) << "err";
        //    break;
        //}
        //if(tcp_info.tcpi_state != TCP_ESTABLISHED) {
        //    SYLAR_LOG_INFO(g_looger)
        //            << " state=" << (int)tcp_info.tcpi_state;
        //    break;
        //}
        static int batch = 10000000;
        if(i && (i % batch) == 0) {
            uint64_t ts2 = fwl::GetTimeUs();
            FWL_LOG_INFO(g_looger) << "i=" << i << " used: " << ((ts2 - ts) * 1.0 / batch) << " us";
            ts = ts2;
        }
    }
}

int main(int argc, char** argv) {
    fwl::IOManager iom;
    iom.scheduler(&test_socket);
    iom.scheduler(&test2);
    return 0;
}
