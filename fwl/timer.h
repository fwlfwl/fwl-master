#ifndef __H_TIMER__
#define __H_TIMER__

/**
 * @file timer.h
 * @brief 定时器类
 * @auth fwl
 * @email 1822363660@qq.com
 * @data 2023-12-3
 * */

#include<set>
#include<vector>
#include<iostream>
#include<memory>
#include<functional>
#include"mutex.h"

namespace fwl{

/**
 * @brief 定时器类
 * */
class Timer{
friend class TimerManager;
public:
    typedef std::shared_ptr<Timer> ptr;
    /**
     * @brief 构造函数1
     * @param[in] next 下次执行时间
     * */
    Timer(uint64_t m_next);
    
    /**
     * @brief 构造函数2
     * */
    Timer(uint64_t ms, std::function<void()> cb, bool isRecurrent);
private:
    //是否循环定时器
    bool m_isRecurrent = false;
    //循环周期
    uint64_t m_ms = 0;
    //下次执行时间
    uint64_t m_next = 0;
    //回调函数
    std::function<void()> m_cb = nullptr;

private:
    //自定义比较仿函数
    struct Comparison{
        bool operator()(const Timer::ptr & lTimer, const Timer::ptr & rTimer) const;
    };
};

/**
 * @brief 定时器管理类
 * */
class TimerManager{
public:
    typedef RWMutex RWMutexType;    
    /**
     * @brief 构造函数
     * */
    TimerManager();

    /**
     * @brief 析构函数
     * */
    virtual ~TimerManager();

    /**
     * @brief 获取下一个定时期时间
     * */
    uint64_t getNextTimer();
    
    /**
     * @brief 新增定时器
     * @param[in] ms 定时器重复时间
     * @param[in] cn 回调函数
     * @param[in] isRecurrent 是否循环定时器
     * */
    Timer::ptr addTimer(uint64_t ms, std::function<void ()> cb, bool isRecurrent = false);
   
	/**
	 * @brief 新增条件定时器
	 * @param[in] ms 定时器超时时间
	 * @param[in] cb 回调函数
	 * @param[in] winfo 执行条件
	 * @param[in] isRecurrent 是否循环定时器
	 * */
	Timer::ptr addConditionTimer(uint64_t ms, std::function<void ()> cb, std::weak_ptr<void> winfo, bool isRecurrent = false);

    /**
     * @brief 取消定时间器
     * @param[in] 需要取消的定时器
     * */
    bool delTimer(Timer::ptr timer);

    /**
     * @brief 重置定时器
     * */
    bool resetTimer(Timer::ptr timer, uint64_t ms, bool isRecurrent);

    /**
     * @brief 超时定时处理
     * */
    void handleExpireTimer(std::vector<std::function<void()> > & cbs);

protected:
    
    /**
     * @brief 新增事件（触发时间最近，即加在同步）调用函数
     * */
    virtual void onFrontEvent() =  0;

    /**
     * @brief 添加定时器到管理器中
     * @
     * */

private:
    //锁
    RWMutexType m_mutex;
    //定时器集合
    std::set<Timer::ptr,Timer::Comparison> m_timers;
    //判断是否触发最新事件定时器（即新事件是最近时间）
    bool m_ticked = false;
};

}

#endif


