#include"../fwl/unit.h"
#include"../fwl/log.h"
#include"../fwl/config.h"
#include"../fwl/thread.h"
#include"../fwl/mutex.h"

static fwl::Logger::ptr logger = FWL_LOG_NAME("system");
static int count;
static fwl::Mutex mutex;

void fun1(){
    fwl::Mutex::Lock lock(mutex);
    for(int i = 0; i <  100000; ++i){
        ++count;
    }
    sleep(30);
    //FWL_LOG_INFO(logger) << "fun1 conut:" << count;
}

void mutex_log1(){
    unsigned long i = 0;
    while(i < 1000000){
        FWL_LOG_INFO(logger) << "++++++++++++++++++++++" << std::to_string(i);
        ++i;
    }
}

void mutex_log2(){
    unsigned long i = 0;
    while(i <  1000000){
        FWL_LOG_INFO(logger) << "----------------------" << std::to_string(i);
        ++i;
    }
}

//测试Thread和Mutex类
void testThread(){
    fwl::Thread::ptr thread[5];
    for(int i = 0; i < 5; ++i){
        thread[i].reset(new fwl::Thread(fun1,"thread_"+std::to_string(i)));
    }
    for(int i = 0; i < 5; ++i){
        thread[i] -> join();
        FWL_LOG_INFO(logger) << "ThreadId:" << thread[i] -> getId()
            << "ThreadName:"<< thread[i] -> GetName();
    }
    FWL_LOG_INFO(logger) << "main count:"<<count;
}

//测试加锁后的log
void testMutexLog(){
    fwl::Thread::ptr t1(new fwl::Thread(mutex_log1,"thread_1"));
    fwl::Thread::ptr t2(new fwl::Thread(mutex_log2,"thread_1"));
    t1 -> join();
    t2 -> join();
}


int main(){
    YAML::Node root = YAML::LoadFile("../log.yml");
    fwl::Config::loadYamlConfig(root);
    //testThread();
    testMutexLog();
    return 0;
}

#undef XX
#undef XXM
#undef XXMVP
