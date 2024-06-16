#include<atomic>
#include<iostream>
#include"fiber.h"
#include "scheduler.h"
#include"macro.h"
#include"config.h"
#include<functional>

namespace fwl{

#define DEFAULT_STACK_SIZE 1024*1024

//局部宏定义
#undef GETCONTEXT
#define GETCONTEXT(x) \
    if( 0 != getcontext((x))){ \
        ASSERT_PRE(false,"getcontext"); \
    } 

#undef UCONTEXT_T_INIT
#define UCONTEXT_T_INIT(ctx,stack,stack_size,link) \
    {   \
        ctx.uc_stack.ss_sp = stack; \
        ctx.uc_stack.ss_size = stack_size;  \
        ctx.uc_link = link;  \
    }   

#undef UCONTEXT_INIT
#define UCONTEXT_INIT(ctx,stack,stack_size,link,MainRun) \
    {   \
        GETCONTEXT(&ctx)    \
        UCONTEXT_T_INIT(m_ctx,stack,stack_size,link) \
        makecontext(&ctx, MainRun, 0);   \
    }   

#undef SWAPCONTEXT
#define SWAPCONTEXT(oucp,ucp)   \
    if(0 != swapcontext(oucp,ucp)){ \
        ASSERT_PRE(false,"swapcontext"); \
    }

//logger
static Logger::ptr g_logger = FWL_LOG_NAME("system");

//读取配置文件Fiber堆大小
ConfigVar<size_t>::ptr g_config_stack = Config::lookUp("system.statck_size",size_t(DEFAULT_STACK_SIZE), "stack size");

//当前协程
static thread_local Fiber* t_fiber = nullptr;
//线程主协程
static thread_local Fiber::ptr t_thread_fiber = nullptr;
//协程id
static std::atomic<uint64_t> t_fiber_id =  {0};
//协程数
static std::atomic<uint64_t> t_fiber_count =  {0};

//Fiber初始化，获取当前上下文
Fiber::Fiber(const std::string & name):m_id(t_fiber_id++),m_name(name){ 
    m_state = RUNNING;
    SetThis(this);
    FWL_LOG_DEBUG(g_logger) << "Fiber(),FibferId = " << m_id; //<< ",ptr = " << this; 

    //获取当前CPU上下文
    GETCONTEXT(&m_ctx);
}

//Fiber初始化，设置上下文
Fiber::Fiber(std::function<void()> cb, bool user_call, size_t stacksize, const std::string & name):
    m_id(t_fiber_id++),m_name(name),m_cb(cb){
    m_state = RUNNABLE;
    ++t_fiber_count;
    FWL_LOG_DEBUG(g_logger) << "Fiber(X,X,X),FiberId = " <<  m_id; //<< ",ptr = " << this; //<< ",FiberName:" <<
        //(m_name.empty() ? "null" : m_name);
    
    //分配栈空间
    m_stacksize = (0 != stacksize ? stacksize : g_config_stack -> getValue());
    m_stack = Allocater::allocate(m_stacksize);
	if(user_call){
    	UCONTEXT_INIT(m_ctx, m_stack, m_stacksize, nullptr, &Fiber::SchedulerMainRun)
	}else{
    	UCONTEXT_INIT(m_ctx, m_stack, m_stacksize, nullptr, &Fiber::MainRun)
	}
}

Fiber::~Fiber(){
    if(m_stack){
		ASSERT_PRE(INIT == m_state || TERM == m_state || EXPECT == m_state, "deallocate statck");
    	Allocater::deallocate(m_stack);
	}else{
		ASSERT(!m_cb);
		Fiber * cur = t_fiber;
		if(cur == this){
			SetThis(nullptr);
		}
	}
    --t_fiber_count;
    FWL_LOG_DEBUG(g_logger) << "~Fiber(),FiberId:" <<  m_id  << " exit";//<< ",FiberName:" << 
        //(m_name.empty() ? "null" : m_name);
}

bool Fiber::reset(std::function<void()> new_cb){
	m_state = RUNNABLE;
    if(!new_cb){
        m_cb = nullptr;
        m_state = TERM;
        return false;
    }
    //新运行函数
    m_cb = new_cb;
    UCONTEXT_INIT(m_ctx,m_stack,m_stacksize,nullptr,MainRun)
    m_state = RUNNABLE;
    return true;
}

void Fiber::SwapOut(){
    SetThis(Scheduler::GetSchedulerFiber());  
    //如果运行状态，则将状态切换为可运行状态
    SWAPCONTEXT(&m_ctx,&Scheduler::GetSchedulerFiber() -> m_ctx);
}

void Fiber::SwapIn(){
    //ASSERT(RUNNING !=  m_state);
    SetThis(this);
    m_state = RUNNING;
	SWAPCONTEXT(&Scheduler::GetSchedulerFiber() -> m_ctx, &m_ctx);
}

/**
* @切换至目标协程
* */
bool Fiber::back(){
    SetThis(t_thread_fiber.get());  
    //如果运行状态，则将状态切换为挂起状态
    if(TERM != m_state && EXPECT != m_state){
        m_state = RUNNABLE;
    }
    SWAPCONTEXT(&m_ctx,&t_thread_fiber.get() -> m_ctx);
    return true;
}

/**
* @从目标协程切换回来
* */
bool Fiber::call(){
    ASSERT(RUNNING !=  m_state);
    SetThis(this);
    m_state = RUNNING;
    SWAPCONTEXT(&t_thread_fiber.get() -> m_ctx, &m_ctx);
    return true;
}

uint32_t Fiber::GetId(){
    if(t_fiber){
        return t_fiber -> m_id;
    }
    return 0;
}
std::string Fiber::GetName(){
    if(t_fiber){
        return t_fiber -> m_name;
    }
    return "";
}

//设置当前Fiber
void Fiber::SetThis(Fiber * f){
    t_fiber = f;
}

//设置协程状态
void Fiber::SetState(Fiber::State in_state){
    m_state = in_state;
}

//返回协程状态
Fiber::State Fiber::GetState() const{
    return m_state;
}

//获取当前Fiber
Fiber::ptr Fiber::GetThis(){
    if(t_fiber){
        return t_fiber -> shared_from_this();
    }
	Fiber::ptr main_fiber(new Fiber);
	ASSERT(t_fiber == main_fiber.get());
	t_thread_fiber = main_fiber;
	return t_fiber -> shared_from_this();
}

//切换至挂起状态
void Fiber::SwitchToSUSPEND(){
    auto cur = GetThis();
    if(TERM != cur -> m_state && EXPECT != cur -> m_state){
        cur -> m_state = SUSPEND;
        cur -> SwapOut();
    }
}

//切换至准备运行状态
void Fiber::SwitchToRUNNABLE(){
    auto cur = GetThis();
    if(TERM != cur -> m_state && EXPECT != cur -> m_state){
        cur -> m_state = RUNNABLE;
        cur -> SwapOut();
    }
}


//主运行函数
void Fiber::MainRun(){
//    FWL_LOG_DEBUG(g_logger) << "run MainRun()";
    auto cur = GetThis().get();
    try{
        cur -> m_state = RUNNING;
		cur -> m_cb();
        cur -> m_cb = nullptr;
        cur -> m_state = TERM;
    }catch(std::exception & e){
        cur -> m_state = EXPECT;
        FWL_LOG_ERROR(g_logger) << "Fiber which id is "<<  cur -> m_id
            << " running error " << e.what() << std::endl
            << BacktraceToString();
    }catch(...){
        cur -> m_state = EXPECT;
        FWL_LOG_ERROR(g_logger) << "Fiber which id is "<<  cur -> m_id
            << "running error " << std::endl
            << BacktraceToString();
    } 
    cur -> SwapOut();
    FWL_LOG_DEBUG(g_logger) << "test can not reach here";
}

//调度线程主运行函数
void Fiber::SchedulerMainRun(){
//    FWL_LOG_DEBUG(g_logger) << "run SchedulerMainRun()";
    auto cur = GetThis().get();
    try{
        cur -> m_state = RUNNING;
		cur -> m_cb();
        cur -> m_cb = nullptr;
        cur -> m_state = TERM;
    }catch(std::exception & e){
        cur -> m_state = EXPECT;
        FWL_LOG_ERROR(g_logger) << "Fiber which id is "<<  cur -> m_id
            << " running error " << e.what() << std::endl
            << BacktraceToString();
    }catch(...){
        cur -> m_state = EXPECT;
        FWL_LOG_ERROR(g_logger) << "Fiber which id is "<<  cur -> m_id
            << "running error " << std::endl
            << BacktraceToString();
    } 
    cur -> back();
    FWL_LOG_DEBUG(g_logger) << "test can not reach here";
}

}


#undef DEFAULT_STACK_SIZE
#undef GETCONTEXT
#undef UCONTEXT_T_INIT
#undef UCONTEXT_INIT
