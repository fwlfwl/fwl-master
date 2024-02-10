#ifndef __H_IOMANAGER__
#define __H_IOMANAGER__

/**
* @file iomanager.h
* @brief IO模块封装
* @author fwl
* @email 1822363660@qq.com
* @data 2023-11-01
*/

#include<vector>
#include"scheduler.h"
#include"mutex.h"
#include"timer.h"

namespace fwl{

/**
 * @brief IO模块封装
 * */
class IOManager : public Scheduler, public TimerManager{
public:
enum Event{
    NONE = 0x00,
    READ = 0x01,
    WRITE = 0x04
};

private:
    /**
     * @brief 事件器-绑定fd和调度器
     * */
    struct FdContext{
        typedef Mutex MutexType;
        struct EventContext{
            //事件调度器
            Scheduler * m_sc = nullptr;
            //事件调用协程
            Fiber::ptr m_fiber;
            //事件调度函数
            std::function<void()> m_cb = nullptr;
        };

        //获取事件
        EventContext& GetContext(Event event);

        /**
         * @brief 重置事件
         * @param[in] event 需重置事件
         * */
        void ResetContext(EventContext& event);
        
         /**
         * @brief 重置当前读写事件
         * */
        void ResetContext();

         /**
         * @brief 触发事件
         * */
        void TriggerContext(Event event);

        //事件关联句柄
        int m_fd = -1;
        //读事件
        EventContext m_readCtx;
        //写事件
        EventContext m_writeCtx;
        //事件
        Event m_event = NONE;
        //锁
        MutexType m_mutex;
    };

public:
    typedef RWMutex RWMutexType;

    //构造函数
    IOManager(size_t threadNum =  1, bool use_call = true, const std::string & name = "");

    //析构函数
    ~IOManager();

    /**
     * @brief 添加事件
     * @param[in] fd 添加句柄
     * @param[in] event 事件类型-读/写
     * @param[in] cb 执行函数
     * */   
    int addEvent(int fd, Event event, std::function<void()> cb = nullptr);
    
    /**
     * @brief 删除事件
     * @param[in] fd 句柄
     * @param[in] event 事件类型-读/写
     * */
    bool delEvent(int fd, Event event);

    /**
     * @brief 取消事件
     * @param[in] fd 句柄
     * @param[in] event 事件类型-读/写
     * */
    bool cancelEvent(int fd,Event event);

    /**
     * @brief 取消所有事件
     * */
    bool cancelAll(int fd);
    
    /**
     * @brief 返回当前IOManager
     * */
    static IOManager * GetThis();
protected:
    /**
     * @brief通知调度函数
     */
    void tick() override;

    /**
     * @brief 待定时器的stopping函数
     * @param[in] timeout 超时时间
     */
    bool stopping(uint64_t & timeout);

    /**
     * @berief stopping函数
     * */
    bool stopping() override;
    
    /**
     * @berief 无任务运行函数
     * */
    void idle() override;

    /**
     * @brief onFrontEvent函数
     * */
    void onFrontEvent() override;

private:
    /**
     * @brief epoll事件操作
     * @param[in] epollfd epoll句柄
     * @param[in] fd 事件句柄
     * @param[in] op epoll操作
     * @param[in] event 读/写事件
     * @param[in] enable_et ET操作是否允许
     * */
    void opfd(int epollfd, int fd, int op, int event, bool enable_et = true);

    /**
     * @brief 关闭打开已打开句柄
     * */
    void closeAll();
    
    /**
     * @brief 重新分配事件器大小
     * */
    void resize(size_t size);

private:
    //epoll描述符
    int m_epollfd;
    //信号管道，通知处理事件
    int m_tickPipefd[2];
    //事件器
    std::vector<FdContext *> m_fdContexts; 
    //待执行的事件数
    std::atomic<uint32_t> m_ready_eventNum =  {0};
    //局部锁
    RWMutexType m_mutex;
};

}
#endif


