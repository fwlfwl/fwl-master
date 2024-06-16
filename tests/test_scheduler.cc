#include "../fwl/scheduler.h"
#include"../fwl/log.h"
#include"../fwl/mutex.h"
#include<sys/epoll.h>

static fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

void test_fiber(){
    static int count =  5;
    FWL_LOG_INFO(g_logger) << "test_fiber(),count:"<< count;
    sleep(1);
    if(--count >  0){
        //FWL_LOG_DEBUG(g_logger) << "run here";
		fwl::Scheduler::GetThis() -> scheduler(&test_fiber);//,fwl::getThreadId());
    }
}

int main()
{
    //FWL_LOG_INFO(g_logger) << EPOLLIN << " " << EPOLLOUT;
    fwl::Scheduler sc(3,true,"main");
    sc.start();
    sc.scheduler(&test_fiber);
    sc.stop();
    return 0;
}

