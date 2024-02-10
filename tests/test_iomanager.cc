#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include "../fwl/iomanager.h"
#include "../fwl/log.h"
#include "../fwl/unit.h"

fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

int sockfd = -1;

void test_iomanager(){
	FWL_LOG_INFO(g_logger) << "______test_iomanager_______";
	fwl::IOManager iom(1);
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    fwl::setnonblocking(sockfd);
    ASSERT(0 <= sockfd);
    
    struct sockaddr_in address;
    memset(&address,0,sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,"127.0.0.1",&address.sin_addr);
    address.sin_port = htons(8080);
    if(0 != connect(sockfd, (struct sockaddr*)&address,sizeof(address))){
        if(errno == EINPROGRESS){
            FWL_LOG_INFO(g_logger) << "here,sokfd:" << sockfd;
            ASSERT(fwl::IOManager::GetThis() != nullptr);
			fwl::IOManager::GetThis() -> addEvent(sockfd, fwl::IOManager::READ, [](){
                                //send(sockfd,"hello",6,0);
                                FWL_LOG_INFO(g_logger) << "read callback";
                            });
            fwl::IOManager::GetThis() -> addEvent(sockfd, fwl::IOManager::WRITE, [](){
                                //send(sockfd,"hello",6,0);
                                FWL_LOG_INFO(g_logger) << "wirte callback";
                            });
        }
    }    
}

void test_iomanager_main(){
    
    fwl::IOManager iom(3);
    FWL_LOG_INFO(g_logger) << "EPOLL_CTL_ADD:" << EPOLL_CTL_ADD 
        << ",EPOLL_CTL_MOD:" << EPOLL_CTL_MOD
        << ",EPOLL_CTL_DEL:" << EPOLL_CTL_DEL;
    iom.scheduler(test_iomanager);
}

fwl::Timer::ptr new_timer = nullptr;
static int i =  0;
void test_timer(){
	FWL_LOG_INFO(g_logger) << "______test_timer_______";
    fwl::IOManager iom(3);
    FWL_LOG_DEBUG(g_logger) << "_______add timer";
    new_timer = iom.addTimer(5000, [&iom](){
                        FWL_LOG_DEBUG(g_logger) << "_______hello timer i:" << i;
                        if(++i ==  3){
                            iom.resetTimer(new_timer,2000,false);
                        }
                    },true);
}

int main()
{
    test_iomanager();
	test_timer();
    return 0;
}

