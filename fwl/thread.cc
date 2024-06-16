#include "thread.h"
#include"log.h"
#define DEFAULT_NAME "UNKNOWN"
#define LOG_NAME "system"

namespace fwl{
//当前运行变量
static thread_local Thread * local_thread = nullptr;
static thread_local std::string local_name = DEFAULT_NAME;

Thread::Thread(std::function<void()> in_cb, const std::string & in_name):
    m_name(in_name),
    m_cb(in_cb){
    if(m_name.empty()){
        m_name = DEFAULT_NAME;
    }
    int ret = pthread_create(&m_pthread,nullptr,&Thread::run,this);
    if(0 != ret){
        FWL_LOG_ERROR(FWL_LOG_NAME(LOG_NAME)) <<  "pthread create failed, errno:" << ret
            << " name:" << m_name;
        throw std::logic_error("ptread_create error");
    }
    m_sem.wait();
}

Thread::~Thread(){
    if(m_pthread){
        int ret = pthread_detach(m_pthread);
        if(0 != ret){
            FWL_LOG_ERROR(FWL_LOG_NAME(LOG_NAME)) << "pthread detach error, errno:" << ret
                << " name" << m_name;
        }
    }
}

void Thread::join(){
    if(m_pthread){
        int ret = pthread_join(m_pthread,nullptr);
        if(0 != ret){
            FWL_LOG_ERROR(FWL_LOG_NAME(LOG_NAME)) << "pthread detach error, errno:" << ret
                << " name" << m_name;
            throw std::logic_error("pthread_join error");
        }
    }
    m_pthread = 0;
}

/**
* @brief 设置当前线程名
* */
void Thread::SetName(const std::string & name){ 
    if(name.empty()){
        return;
    }
    if(local_thread){
        local_thread -> m_name = name;
    }
    local_name = name;
};
    
/**
* @beief 获取当前线程名
* */
std::string Thread::GetName(){
    return local_name;
};

/**
* @brief 返回当前线程
* */
Thread * GetThis(){
    return local_thread; 
}

void * Thread::run(void * arg){
    Thread * th = (Thread *)arg;
    local_thread = th;
    local_name = th -> m_name;
    local_thread -> m_id = fwl::getThreadId();

    std::function<void()> cb;
    cb.swap(th -> m_cb);
    th -> m_sem.notify();
    cb();
    return 0;
}

}
