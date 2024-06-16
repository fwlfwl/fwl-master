#include "fwl/hook.h"
#include "fwl/log.h"
#include "fwl/iomanager.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

fwl::Logger::ptr g_logger = FWL_LOG_ROOT();

void test_sleep() {
    FWL_LOG_INFO(g_logger) << "_____test_sleep_____";
    fwl::IOManager iom(2);
    iom.scheduler([](){
        sleep(2);
        FWL_LOG_INFO(g_logger) << "sleep 2";
    });
    iom.scheduler([](){
        sleep(3);
        FWL_LOG_INFO(g_logger) << "sleep 3";
    });
    FWL_LOG_INFO(g_logger) << "test_sleep end";
}

void test_sock() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    inet_pton(AF_INET, "36.155.132.3", &addr.sin_addr.s_addr);

    FWL_LOG_INFO(g_logger) << "begin connect";
    int rt = connect(sock, (const sockaddr*)&addr, sizeof(addr));
    FWL_LOG_INFO(g_logger) << "connect rt=" << rt << " errno=" << errno;

    if(rt) {
        return;
    }

	fwl::set_hook_enable(true);
    const char data[] = "GET / HTTP/1.0\r\n\r\n";
    rt = send(sock, data, sizeof(data), 0);
	FWL_LOG_INFO(g_logger) << "send rt=" << rt << " errno=" << errno;

    if(rt <= 0) {
        return;
    }

    std::string buff;
    buff.resize(40960);

	FWL_LOG_DEBUG(g_logger) << "recv";
    rt = recv(sock, &buff[0], buff.size(), 0);
    FWL_LOG_INFO(g_logger) << "recv rt=" << rt << " errno=" << errno;

    if(rt <= 0) {
        FWL_LOG_ERROR(g_logger) << "Do not recv";
		return;
    }

    buff.resize(rt);
    FWL_LOG_INFO(g_logger) << "Recv content:" << buff;
}

int main(int argc, char** argv) {
//    test_sleep();
    FWL_LOG_INFO(g_logger) << "_____test_sock_____";
	fwl::IOManager iom(2);
    iom.scheduler(test_sock);
    return 0;
}
