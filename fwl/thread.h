#ifndef _H_Thread_
#define _H_Thread_

/**
* @file pthread.h
* @brief 自定义线程封装
* @author fwl
* @email 1822363660@qq.com
* @data 2023-11-03
*/

#include"base_head.h"
#include"unit.h"
#include"noncopy.h"
#include"mutex.h"
#include<pthread.h>

namespace fwl{

/**
 * @brief 自定义线程类
 * */
class Thread : NonCopy
{
public:
    typedef std::shared_ptr<Thread> ptr;

    /**
     * @brief 构造函数
     * */
    Thread(std::function<void()> cb,const std::string & name = "");

    /**
     * @brief 稀构函数
     * */
    ~Thread();

    /**
     * @beief 获取线程名
     * */
    std::string getName() const { return m_name;  };

    /**
     * @beief 获取进程ID
     * */
    pid_t getId() const{ return m_id; }

    /**
     * @brief 等待线程执行完
     * */
    void join();

public:
    /**
     * @brief 返回当前线程
     * */
    static Thread * GetThis();
    
    /**
     * @brief 设置当前线程名
     * */
    static void SetName(const std::string & name);
    
    /**
     * @brief 获取当前线程名
     * */
    static std::string GetName();

private: 
    /**
     * @brief 运行函数
     * */
    static void * run(void *);

private:
    //进程ID
    pid_t m_id;
    //线程结构
    pthread_t m_pthread;
    //线程名
    std::string m_name;
    //线程执行函数
    std::function<void()> m_cb;
    //信号量
    Semaphore m_sem;
};

}
#endif
