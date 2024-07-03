#include "timer.h"
#include "unit.h"
#include "log.h"

#define NO_TIMER (~0ul)

namespace fwl{

static Logger::ptr g_logger = FWL_LOG_NAME("system");

//Timer实现类

//自定义比较函数
bool Timer::Comparison::operator()(const Timer::ptr & lTimer, const Timer::ptr & rTimer) const{
    //判断定时器是否存在
    if(!lTimer){
        return false;
    }else if(!rTimer){
        return true;
    }

    //比较下次执行时间
    if(lTimer -> m_next < rTimer -> m_next){
        return true;
    }else if(lTimer -> m_next > rTimer -> m_next){
        return false;
    }

    //比较时间周期
    if(lTimer -> m_ms < rTimer -> m_ms){
        return true;
    }

    //都一样，则比较指针，小的优先
    return lTimer.get() < rTimer.get();
}

/**
* @brief 构造函数1
* @param[in] next 下次执行时间
* */
Timer::Timer(uint64_t next):m_next(next){  
}
    
/**
* @brief 构造函数2
* */
Timer::Timer(uint64_t ms, std::function<void()> cb, bool isRecurrent):
    m_isRecurrent(isRecurrent),
    m_ms(ms),
    m_cb(cb){
        //计算执行时间
        uint64_t now_ms = GetTimeMs();
        m_next = now_ms + m_ms;
}

//TimerManager类实现

/**
 * @brief 构造函数
 * */
TimerManager::TimerManager(){}

/**
 * @brief 析构函数
 * */
TimerManager::~TimerManager(){}

/**
* @brief 新增定时器
* @param[in] ms 定时器重复时间
* @param[in] cn 回调函数
* @param[in] isRecurrent 是否循环定时器
* */
Timer::ptr TimerManager::addTimer(uint64_t ms, std::function<void ()> cb, bool isRecurrent){
    Timer::ptr new_timer(new Timer(ms, cb, isRecurrent));
    if(!new_timer){
        return nullptr;
    }
    RWMutexType::WriteLock lock(m_mutex);
    auto it = m_timers.insert(new_timer).first;
    bool at_front = (it == m_timers.begin()) & !m_ticked;
    //判断如果是首个元素，则需要执行函数通 知
    if(at_front){
        onFrontEvent();
    }
    return new_timer;
}

/**
 * @brief 新增条件定时器
 * @param[in] ms 定时器超时时间
 * @param[in] cb 回调函数
 * @param[in] winfo 执行条件
 * @param[in] isRecurrent 是否循环定时器
 * */
Timer::ptr TimerManager::addConditionTimer(uint64_t ms, std::function<void ()> cb, std::weak_ptr<void> winfo, bool isRecurrent){
	if(winfo.lock()){
		return addTimer(ms, cb, isRecurrent);
	}
	return addTimer(ms, nullptr, isRecurrent);
}

 /* @brief 重置定时器
* */
bool TimerManager::resetTimer(Timer::ptr timer, uint64_t ms, bool isRecurrent){
    if(!timer){
        FWL_LOG_DEBUG(g_logger) << "timer is NULL";
        return false;
    }
    
    if(~0ul == ms){
        FWL_LOG_DEBUG(g_logger) << "new timeout is error";
        return false;
    }
    
    {
        //找不到则退出
        RWMutexType::ReadLock rLock(m_mutex);
        auto it = m_timers.find(timer);
        if(it == m_timers.end()){
            FWL_LOG_DEBUG(g_logger) << "can not find timer";        
            return false;
        }
    }
    
    RWMutexType::WriteLock wLock(m_mutex);
    m_timers.erase(timer);   
    
    //重置timer
    timer -> m_ms = ms;
    uint64_t now_ms = GetTimeMs();
    timer -> m_next = ms + now_ms;
    timer -> m_isRecurrent = isRecurrent;

       
    auto it = m_timers.insert(timer).first;
    bool at_front = (it == m_timers.begin()) & !m_ticked;
    //判断如果是首个元素，则需要执行函数通 知
    if(at_front){
        //FWL_LOG_DEBUG(g_logger) << "run here";
		onFrontEvent();
    }
    return true;

}

/**
* @brief 取消定时间器
* @param[in] 需要取消的定时器
* */
bool TimerManager::delTimer(Timer::ptr timer){
    {
        //找不到则退出
        RWMutexType::ReadLock rLock(m_mutex);
        auto it = m_timers.find(timer);
        if(it == m_timers.end()){
            return false;
        }
    }
    
    RWMutexType::WriteLock wLock(m_mutex);
    m_timers.erase(timer);
    return true;
}

/**
* @brief 超时定时处理
* */
void TimerManager::handleExpireTimer(std::vector<std::function<void()> > & cbs){ 
	{
		//加读锁
    	RWMutexType::ReadLock rLock(m_mutex);
    	//没有定时器任务则跳出
    	if(m_timers.empty()){
        	return;
    	}
	}

	//加写锁
	RWMutexType::WriteLock wLock(m_mutex);
	if(m_timers.empty()){
		return;
	}
    uint64_t next_ms = (*m_timers.begin()) -> m_next; 
    uint64_t now_ms = GetTimeMs();
    //若下次定时器事件大于当前时间，则跳出
    if(next_ms >  now_ms){
        return;
    }
    
    Timer::ptr now_timer(new Timer(now_ms));
    //返回超过当前时间的首个Timer
    auto it = m_timers.upper_bound(now_timer);

    std::vector<Timer::ptr> expired;
    //保存过期定时器
    expired.insert(expired.begin(), m_timers.begin(), it);
    //删除定时器过期的定时器
    m_timers.erase(m_timers.begin(), it);

    /**
     * 1.保存到要执行的数组cbs
     * 2.循环定时器重新添加到定时器集合中
     */
    for(auto & expired_it : expired){
        cbs.push_back(expired_it -> m_cb);
        if(expired_it -> m_isRecurrent){
            now_ms = GetTimeMs();
            expired_it -> m_next = now_ms + expired_it -> m_ms;
            m_timers.insert(expired_it);
        }
    }
    return;
}

/**
* @brief 获取下一个定时期时间
* */
uint64_t TimerManager::getNextTimer(){
    RWMutexType::ReadLock rLock(m_mutex);
    if(m_timers.empty()){
        return NO_TIMER;
    }
    uint64_t now_ms = GetTimeMs();
    uint64_t next_ms = (*m_timers.begin()) -> m_next;

    return now_ms >= next_ms ? 0 : (next_ms - now_ms);
}


}
