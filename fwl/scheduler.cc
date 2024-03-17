#include"scheduler.h"
#include"thread.h"
#include"macro.h"
#include"log.h"
#include"fiber.h"
#include"unit.h"
#include"macro.h"

#define MAIN_THREAD_NAME "mainthread"

namespace fwl{

//日志打印器
static Logger::ptr g_logger = FWL_LOG_NAME("system");

//主协程
static thread_local Fiber::ptr t_thread_fiber = nullptr;
//当前调度器协程
static thread_local Fiber *  t_scheduler_fiber = nullptr;
//当前调度器
static thread_local Scheduler * t_scheduler =  nullptr;

/**
 * @brief 构造函数
 * @param[in] threadNum 进程数量
 * @param[in] user_call 是否调度协程
 * @param[in] name 进程&调度器名
 * */
Scheduler::Scheduler(size_t threadNum, bool usr_call, const std::string & name):m_name(name){   
    ASSERT(threadNum >  0);
    if(!t_thread_fiber){
        t_thread_fiber.reset(new Fiber);
        Fiber::SetThis(t_thread_fiber.get());
    }
    if(usr_call){
        //生成主协程
        ASSERT(nullptr == GetThis());
        --threadNum;
        //设置当前调度器
        t_scheduler = this;
        //调度器调度协程
        m_rootFiber.reset(new Fiber(t_thread_fiber.get(),std::bind(&Scheduler::run,this)));
        //设置当前调度器的调度协程
        t_scheduler_fiber = m_rootFiber.get();
        
        m_rootThread = fwl::getThreadId();
        m_threadIds.push_back(m_rootThread);
    }else{
        m_rootThread =  -1;
    }
    Thread::SetName(m_name);
    m_threadNum = threadNum;
}

/**
 * @brief 析构函数
 * */
Scheduler::~Scheduler(){
	Fiber::GetThis() -> SetState(TERM); 
	if(stopping()){
        t_scheduler = nullptr;
        t_thread_fiber.reset();
    }

}

/**
 * @brief 开始运行
 * */
void Scheduler::start(){
    FWL_LOG_INFO(g_logger) << "scheduler start()";
    MutexType::Lock lock(m_mutex);
    if(stopping()){
        return;
    }
    m_threads.resize(m_threadNum,nullptr);
    //生成线程池
	for(size_t i =  0; i < m_threadNum; ++i){\
        m_threads[i].reset(new Thread(std::bind(&Scheduler::run,this),"thread" + std::to_string(i)));
    }
    //停止标志设置为false
    m_stop = false;
    
}

/**
 * @brief 停止运行
 * */
void Scheduler::stop(){
    m_auto_stop = true;
    
    /**
     * @brief m_rootFiber存在退出条件
     * */
    if(m_rootFiber && (m_rootFiber -> GetState() == TERM || m_rootFiber -> GetState() ==  EXPECT)){
        m_stop = true;
        return;
    }
    /**
     * @brief 判断是否主Fiber
     * */
    if(-1 != m_rootThread){
        ASSERT(GetThis() == this);
    }else{
        ASSERT(GetThis() !=  this);
    }

    m_stop =  true;
    
    /**
     * @brief 未达到退出条件，且rootFiber非空，则继续执行rootFiber
     * */
    if(!stopping() && m_rootFiber){
		m_rootFiber -> SwapIn();
    }

    std::vector<Thread::ptr> thr;
    {
        MutexType::Lock lock(m_mutex);
        thr.swap(m_threads);
    }
    
    for(auto it : thr){
        it -> join();
        //FWL_LOG_DEBUG(g_logger) << "thread join()";
    }
    
    FWL_LOG_DEBUG(g_logger) << "Scheduler::stop()";
}

/**
 * @brief 无工作下线程运行函数
 * */
void Scheduler::idle(){
    FWL_LOG_INFO(g_logger) << "idle";
    /*while(!IsStopped()){
        //切换到后台执行
        Fiber::SwitchToSUSPEND();
    }*/
}

/**
 * @brief 唤醒函数
 * */
void Scheduler::tick(){
    FWL_LOG_INFO(g_logger) << "tick()";
}

/**
* @brief 确认是否需要停止
* */
bool Scheduler::stopping(){
    return 0 == m_active_threadNum && m_fibers.empty() && m_stop && m_auto_stop;
}

/**
 * @brief 主运行函数
 * */
void Scheduler::run(){
    FWL_LOG_INFO(g_logger) << "Scheduler:run()";
    //设置当前调度器
    SetThis();

    //FWL_LOG_INFO(g_logger) << &t_thread_fiber;
    if(fwl::getThreadId() != m_rootThread){
        //非调度协程，则生成新的主线程
		t_thread_fiber.reset(new Fiber);
        t_scheduler_fiber = t_thread_fiber.get();
    }
    ASSERT(nullptr != t_thread_fiber);

    //创建一个空闲Fiber
    Fiber::ptr IdleFiber(new Fiber(t_scheduler_fiber,std::bind(&Scheduler::idle,this),0,"idle"));
    FiberAndThread fat;
    
    Fiber::ptr fiber = nullptr;
    
    while(true){
        
        bool is_active = false;
        bool is_tick  = false;
        
        fat.reset();
        //防止死锁,调用scheduler会加锁
        {
            MutexType::Lock lock(m_mutex);
  			//FWL_LOG_DEBUG(g_logger) << "Fiber num:" <<  m_fibers.size();  
            auto it = m_fibers.begin();
            while(it != m_fibers.end()){
                //判断是否指定了线程执行
                if(it -> thread !=  -1 && it -> thread != fwl::getThreadId()){
                    //满足条件设置唤醒
                    is_tick = true;
                    ++it;
                    continue;
                }
                //判断同时是fiber和function，返回报错
                if(it -> fiber && it -> fc){
                    FWL_LOG_ERROR(g_logger) << "fiber and function exist at the same time";
                        continue;
                }    
            
                //暂停状态不执行
                if(it -> fiber && it -> fiber -> GetState() == TERM){
                    ++it;
                    continue;
                }
            
                //设置待执行协程
                fat = *it;
                m_fibers.erase(it);
                ++m_active_threadNum;
                is_active = true;
                break;
            }
        }
        //唤醒指定线程-当前条件:非绑定协程的线程
        if(is_tick){
            FWL_LOG_INFO(g_logger) << "is_tick ==  true";
            tick();
        }
        
        //判断是否需要运行
        if(is_active){
            if(fat.fiber && (TERM != fat.fiber -> GetState() 
                             && EXPECT != fat.fiber -> GetState())){    //fiber存在，执行
                fat.fiber -> resetMainFiber(t_scheduler_fiber);
                fat.fiber -> SwapIn();
                //待运行状态，则执行调度器
                if(fat.fiber -> GetState() == RUNNABLE){
                    scheduler(fat.fiber);
                }else if(EXPECT != fat.fiber -> GetState() && TERM != fat.fiber -> GetState()){
                    fat.fiber -> SetState(SUSPEND); 
                }
            }else if(fat.fc){   //function存在，执行
                if(fiber){
                    fiber -> resetMainFiber(t_scheduler_fiber);
                    fiber -> reset(fat.fc);
                }else{
                    fiber.reset(new Fiber(t_scheduler_fiber,fat.fc));
                }
                fiber -> SwapIn();
                //运行后结果，待运行状态，则执行调度器
                if(RUNNABLE == fiber -> GetState() || RUNNING == fiber -> GetState()){
                    scheduler(fiber);
                    fiber.reset();
                }else if(EXPECT == fiber -> GetState() || TERM == fiber -> GetState()){ //终止Fiber
                    fiber -> reset(nullptr);
                    fiber -> resetMainFiber(nullptr);
                }
            }
            --m_active_threadNum; //运行进程数减1
        }else{  //执行空闲函数
            if(TERM == IdleFiber -> GetState()){
                //FWL_LOG_DEBUG(g_logger) << "IdlerFiber is end";
                break;
            }

            ++m_idle_threadNum;
            IdleFiber -> SwapIn();
            --m_idle_threadNum;       
            //if(RUNNING == IdleFiber -> GetState() || RUNNABLE == IdleFiber -> GetState()){
            //}
        }
    }
}

/**
* @berief 设置当前调度器
* */
void Scheduler::SetThis(){
    t_scheduler = this;
}

/**
* @brief 返回当前调度器
* */
Scheduler * Scheduler::GetThis(){
    return t_scheduler;
}

}
