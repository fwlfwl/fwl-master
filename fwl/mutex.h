#ifndef __H_MUTEX__
#define __H_MUTEX__

/**
* @file mutex.h
* @brief 锁封装实现
* @author fwl
* @email 1822363660@qq.com
* @data 2023-11-03
**/

#include"noncopy.h"
#include<stdint.h>
#include<pthread.h>
#include<semaphore.h>

namespace fwl{
/**
 * @brief 信号量封装类
 * */
class Semaphore : NonCopy{
public:
    /**
     * @brief 构造函数
     * @param[value] 初始值
     * */
    Semaphore(uint32_t value =  0);

    /*
     * @brief 稀构函数
     * */
    ~Semaphore();

    /**
     * @brief 信号量+ 1
     * */
    void notify();

    /**
     * @brief 信号量-1
     * */
    void wait();
private:
    sem_t m_semaphore;
};

/**
 * @brief 局部锁模板类
 * */
template<typename T>
class MutexImpl{
public:
    /**
     * @brief 构造函数
     * */
    MutexImpl(T& mutex):m_mutex(mutex){
        m_mutex.lock();
        m_locked = true;
    }

    /**
     * @brief 析构函数
     * */
    ~MutexImpl(){
        m_mutex.unlock();
    }
    
    /**
     * @brief 上锁
     * */
    void lock(){
        if(!m_locked){
            m_mutex.lock();
            m_locked = true;
        }
    }

    /**
     * @brief 解锁
     * */
    void unlock(){
        if(m_locked){
            m_mutex.unlock();
            m_locked =  false;
        }
    }

private:
    //锁
    T& m_mutex;
    //是否上锁
    bool m_locked;
};


/**
 * @brief 局部读锁模板类
 * */
template<typename T>
class RMutexImpl{
public:
    /**
     * @brief 构造函数
     * */
    RMutexImpl(T& rmutex):m_rmutex(rmutex){
        m_rmutex.rdlock();
        m_locked = true;
    }
    
    /**
     * @brief 稀构函数
     * */
    ~RMutexImpl(){
        unlock();
    }

    /**
     * @brief 加锁
     * */
    void lock(){
        if(!m_locked){
            m_rmutex.rdlock();
            m_locked = true;
        }
    }
        
    /**
     * @brief 解锁
     * */
    void unlock(){
        if(m_locked){
            m_rmutex.unlock();
            m_locked = false;
        }
    }
private:
    //读锁
    T & m_rmutex;
    //是否上锁
    bool m_locked;
};

/**
 * @brief 局部写锁模板类
 * */
template<typename T>
class WMutexImpl{
public:
    /**
     * @brief 构造函数
     * */
    WMutexImpl(T& wmutex):m_wmutex(wmutex){
        m_wmutex.wrlock();
        m_locked = true;
    }
    
    /**
     * @brief 稀构函数
     * */
    ~WMutexImpl(){
        unlock();
    }

    /**
     * @brief 加锁
     * */
    void lock(){
        if(!m_locked){
            m_wmutex.wrlock();
            m_locked = true;
        }
    }
        
    /**
     * @brief 解锁
     * */
    void unlock(){
        if(m_locked){
            m_wmutex.unlock();
            m_locked = false;
        }
    }
private:
    //读锁
    T & m_wmutex;
    //是否上锁
    bool m_locked;
};

/**
 * @brief 互斥量锁
 * */
class Mutex : NonCopy{
public:
    //局部锁
    typedef MutexImpl<Mutex> Lock;

    /**
     * @brief 构造函数
     * */
    Mutex() {
        pthread_mutex_init(&m_mutex,nullptr);
    }
    
    /**
     * @brief 稀构函数
     * */
    ~Mutex() {
        pthread_mutex_destroy(&m_mutex);
    }

    /**
     * @brief 加锁
     * */
    void lock(){
        pthread_mutex_lock(&m_mutex);
    }

    /**
     * @brief 释放锁
     * */
    void unlock(){
        pthread_mutex_unlock(&m_mutex);
    }
private:
    pthread_mutex_t m_mutex;
};

/**
 * @brief 自旋锁
 * */
class SpinMutex : NonCopy{
public:
    //局部锁
    typedef MutexImpl<SpinMutex> Lock;

    /**
     * @brief 构造函数
     * */
    SpinMutex() {
        pthread_spin_init(&m_mutex,PTHREAD_PROCESS_PRIVATE);
    }
    
    /**
     * @brief 稀构函数
     * */
    ~SpinMutex() {
        pthread_spin_destroy(&m_mutex);
    }

    /**
     * @brief 加锁
     * */
    void lock(){
        pthread_spin_lock(&m_mutex);
    }

    /**
     * @brief 释放锁
     * */
    void unlock(){
        pthread_spin_unlock(&m_mutex);
    }
private:
    pthread_spinlock_t m_mutex;
};
/**
 * @brief 读写锁
 * */
class RWMutex : NonCopy{
public:
    //局部锁
    typedef RMutexImpl<RWMutex> ReadLock;
    typedef WMutexImpl<RWMutex> WriteLock;

    /**
     * @brief 构造函数
     * */
    RWMutex() {
        pthread_rwlock_init(&m_mutex,nullptr);
    }
    
    /**
     * @brief 稀构函数
     * */
    ~RWMutex() {
        pthread_rwlock_destroy(&m_mutex);
    }

    /**
     * @brief 加读锁
     * */
    void rdlock(){
        pthread_rwlock_rdlock(&m_mutex);
    }

    /**
     * @brief 加写锁
     * */
    void wrlock(){
        pthread_rwlock_wrlock(&m_mutex);
    }
    /**
     * @brief 释放锁
     * */
    void unlock(){
        pthread_rwlock_unlock(&m_mutex);
    }
private:
    pthread_rwlock_t m_mutex;
};

/**
 * @brief 空锁-对比测试加锁效果
 * */
class NullMutex : NonCopy{
public:
    //局部锁
    typedef MutexImpl<NullMutex> Lock;

    /**
     * @brief 构造函数
     * */
    NullMutex() {
    }
    
    /**
     * @brief 稀构函数
     * */
    ~NullMutex() {
    }

    /**
     * @brief 加锁
     * */
    void lock(){
    }

    /**
     * @brief 释放锁
     * */
    void unlock(){
    }
};


}
#endif


