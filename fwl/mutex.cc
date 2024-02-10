#include "mutex.h"
#include"log.h"

namespace fwl{

static Logger::ptr logger = FWL_LOG_NAME("system");

//Semaphore类实现
Semaphore::Semaphore(uint32_t value){
    if(0 != sem_init(&m_semaphore,0,value)){
        FWL_LOG_ERROR(logger) << "sem_init error";
        throw std::logic_error("sem_init error");
    }
}

Semaphore::~Semaphore(){
    if(0 != sem_destroy(&m_semaphore)){
        FWL_LOG_ERROR(logger) << "sem_destroy error";
    }
}

void Semaphore::notify(){
    if(0 != sem_post(&m_semaphore)){
        FWL_LOG_ERROR(logger) << "sem_post error";
        throw std::logic_error("sem_post error");
    }
}

void Semaphore::wait(){
    if(0 != sem_wait(&m_semaphore)){
        FWL_LOG_ERROR(logger) << "sem_wait error";
        throw std::logic_error("sem_wait error");
    }
}

};

