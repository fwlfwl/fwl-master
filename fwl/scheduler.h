#ifndef __H_SCHEDULER__
#define __H_SCHEDULER__

#include <iostream>
#include<atomic>
#include<vector>
#include<functional>
#include"thread.h"
#include"mutex.h"
#include "fiber.h"

namespace fwl{

class Scheduler {
public:
    typedef Mutex MutexType;

    /** 
     * @brief 构造函数
     * @param[in] threadsize 线程数量
     * @param[in] use_call 是否使用当前调用线程
     * @param[in] name 调度器名
     * */
    Scheduler(size_t threadsize =  1, bool use_call = true, const std::string & name = "");

    /**
     * @brief 析构函数
     * */
    virtual ~Scheduler();

    /**
     * @brief 启动调度器
     * */
    void start();

    /**
     * @berief 运行函数
     * */
    void run();

    /**
     * @berief 停止调度器
     * */
    void stop();

    /**
     * @berief 协程调度器（单Fiber添加）
     * */
    template<class FiberOrCb>
    void scheduler(FiberOrCb f, int thread =  -1){
        bool need_tick = false;
        MutexType::Lock lock(m_mutex);
        need_tick = schedulerNolock(f,thread);
        if(need_tick){
			std::cout << "ticK()";
			tick();
        }
    }
    /**
     * @berief 协程调度器（批量添加）
     * */
    template<class FiberOrCbIterator>
    void scheduler(FiberOrCbIterator begin, FiberOrCbIterator end){
        bool need_tick = false;
        MutexType::Lock lock(m_mutex);
        while(begin != end){
            need_tick = schedlulerNolock(&*begin,-1) | need_tick;
        }
        if(need_tick){
          	std::cout << "tick";
			tick();
        }
    }

    /**
     * @brief 返回当前调度器
     * */
    static Scheduler * GetThis();

	//返回调度协程
	static Fiber * GetSchedulerFiber();
protected:
    //通知调度函数
    virtual void tick();

    /**
     * @berief stopping函数
     * */
    virtual bool stopping();
    
    /**
     * @berief 无任务运行函数
     * */
    virtual void idle();

private:
    /**
     * @berief 设置当前调度器
     * */
    void SetThis();

private:
    //协程调度器(无锁)
    template<class FiberOrCb>
    bool schedulerNolock(FiberOrCb f, int th){
        bool need_tick = m_fibers.empty();
        FiberAndThread fat = FiberAndThread(f,th);
        if(fat.fiber || fat.fc){
            m_fibers.push_back(fat);           
        }
        return need_tick;
    }

private:
    /**
     *@brief Fiber对应Thread结构体
     * */
    struct FiberAndThread{
        //协程
        Fiber::ptr fiber;
        //协程函数
        std::function<void()> fc;
        //进程Id
        int thread;
        
        /**
         * @brief 构造函数
         * @param[in] f 协程
         * @param[in] th 线程号
         * */
        FiberAndThread(Fiber::ptr f, int th):fiber(f),thread(th){
        }
        
        /**
         * @brief 构造函数
         * @param[in] f 协程指针
         * @param[in] th 线程号
         * */
        FiberAndThread(Fiber::ptr * f, int th):thread(th){
            fiber.swap(*f);
        }
    
        /**
         * @brief 构造函数
         * @param[in] f 协程函数
         * @param[in] th 线程号
         * */
        FiberAndThread(std::function<void()> f, int th):fc(f),thread(th){
        }
        
        /**
         * @brief 构造函数
         * @param[in] f 协程函数指针
         * @param[in] th 线程号
         * */
        FiberAndThread(std::function<void()> *f, int th):thread(th){
            fc.swap(*f);
        }

        /**
         * @brief 默认构造函数
         * */
        FiberAndThread():fiber(nullptr),fc(nullptr),thread(-1){
        }

        /**
         * @brief 重置结构体
         * */
        void reset(){
            fiber.reset();
            fc = nullptr;
            thread = -1;
        }
    };
private:
    //局部锁
    MutexType m_mutex;
    //线程池
    std::vector<Thread::ptr> m_threads;
    //线程数
    uint32_t m_threadNum =  0;
    //待执行的协程
    std::vector<FiberAndThread> m_fibers;
    //use_call为true,调度协程
    Fiber::ptr m_rootFiber =  nullptr;
    //调度器名
    std::string m_name;

protected:
    //协程可调度线程Id
    std::vector<int> m_threadIds;
    //可调用线程数
    std::atomic<uint32_t> m_active_threadNum = {0};
    //空闲线程数
    std::atomic<uint32_t> m_idle_threadNum = {0};
    //主线程Id
    int m_rootThread;
    //是否停止
    bool m_stop = false;
    //是否自动停止
    bool m_auto_stop = false;
};

}
#endif


