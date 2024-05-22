#include<atomic>
#include<iostream>
#include"fiber.h"
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
static thread_local Fiber* t_Fiber = nullptr;

//协程id
static std::atomic<uint64_t> t_Fiber_id =  {0};

//协程数
static std::atomic<uint64_t> t_Fiber_count =  {0};

//Fiber实现
Fiber::Fiber(const std::string & name):m_id(++t_Fiber_id),m_name(name){ 
    m_state = RUNNING;
    SetThis(this);
    FWL_LOG_DEBUG(g_logger) << "Fiber(),FibferId is " << m_id; 

    //获取当前CPU上下文
    GETCONTEXT(&m_ctx);
}

Fiber::Fiber(Fiber * main_fiber, std::function<void()> cb, size_t stacksize,const std::string & name):
    m_id(++t_Fiber_id),m_name(name),m_cb(cb){
    ASSERT(nullptr != main_fiber);
    m_main_fiber = main_fiber;
    m_state = RUNNABLE;
    SetThis(this);
    ++t_Fiber_count;
    FWL_LOG_DEBUG(g_logger) << "Fiber(X,X,X),FiberId is " <<  m_id ; //<< ",FiberName:" <<
        //(m_name.empty() ? "null" : m_name);
    
    //分配栈空间
    m_stacksize = (0 != stacksize ? stacksize : g_config_stack -> getValue());
    m_stack = Allocater::allocate(m_stacksize);
    UCONTEXT_INIT(m_ctx, m_stack, m_stacksize, nullptr, &Fiber::MainRun)
}

Fiber::~Fiber(){
    SetThis(this);
    if(INIT != m_state && TERM != m_state && EXPECT != m_state){
        ASSERT_PRE(nullptr == m_cb,"~Fiber");
        t_Fiber = nullptr;
    }
    Allocater::deallocate(m_stack);
    --t_Fiber_count;
    FWL_LOG_DEBUG(g_logger) << "~Fiber(),FiberId:" <<  m_id ;//<< ",FiberName:" << 
        //(m_name.empty() ? "null" : m_name);
}

//重置主协程
bool Fiber::resetMainFiber(Fiber * fiber){
    if(fiber){
        //重置主线程
        m_main_fiber = fiber;
        return true;
    }
    return false;
}

bool Fiber::reset(std::function<void()> new_cb){
    //运行状态无法重置
    if(RUNNABLE == m_state || RUNNING == m_state || SUSPEND == m_state){
//将state转化为字符串
#undef STATE_TO_STRING
#define STATE_TO_STRING(state,state_str)  \
        if(state == RUNNABLE){  \
                state_str = "RUNNABLE";  \
        }else if(state == RUNNING){  \
                state_str =  "RUNNING";   \
        }else if(state == SUSPEND){ \
                state_str = "SUSPEND";   \
        }else{  \
                state_str = "";   \
        }
        std::string state_str;
        STATE_TO_STRING(m_state,state_str);
        FWL_LOG_INFO(g_logger) << "Fiber "<< m_id << " which state is "<< state_str << " can not to reset";
#undef STATE_TO_STRING
        return false;
    }
    if(!new_cb){
        m_cb = nullptr;
        m_state = TERM;
        return false;
    }
    //重置m_state状态
    m_state = RUNNABLE;
    //新运行函数
    m_cb = new_cb;
    UCONTEXT_INIT(m_ctx,m_stack,m_stacksize,nullptr,MainRun)
    return true;
}

void Fiber::SwapOut(){
    call(m_main_fiber);
}

void Fiber::SwapIn(){
    back(m_main_fiber);
}

/**
* @切换至目标协程
* @param[in] target_fiber 目标协程序
* */
bool Fiber::call(Fiber * target_fiber){
    ASSERT(target_fiber);
    ASSERT_PRE(target_fiber != this,"back oneself");
    SetThis(this);  
    //如果运行状态，则将状态切换为挂起状态
    if(TERM != m_state && EXPECT != m_state){
        m_state = SUSPEND;
    }
    SWAPCONTEXT(&m_ctx,&target_fiber -> m_ctx);
    return true;
}

/**
* @从目标协程切换回来
* @param[in] target_fiber 目标协程序
* */
bool Fiber::back(Fiber * target_fiber){
    ASSERT(target_fiber);
    ASSERT_PRE(target_fiber != this,"back oneself");
    ASSERT(RUNNING !=  m_state);
    SetThis(this);
    m_state = RUNNING;
    SWAPCONTEXT(&target_fiber -> m_ctx, &m_ctx);
    return true;
}

uint32_t Fiber::GetId(){
    if(t_Fiber){
        return t_Fiber -> m_id;
    }
    return 0;
}
std::string Fiber::GetName(){
    if(t_Fiber){
        return t_Fiber -> m_name;
    }
    return "";
}

//设置当前Fiber
void Fiber::SetThis(Fiber * f){
    t_Fiber = f;
}

//设置协程状态
void Fiber::SetState(Fiber::State in_state){
    m_state = in_state;
}

//返回协程状态
Fiber::State Fiber::GetState(){
    return m_state;
}

//获取当前Fiber
Fiber::ptr Fiber::GetThis(){
    if(t_Fiber){
        return t_Fiber -> shared_from_this();
    }

    return nullptr;
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
    //FWL_LOG_INFO(g_logger) << "run MainRun()";
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
    ASSERT(cur -> m_main_fiber);
    cur -> SwapOut();
    FWL_LOG_DEBUG(g_logger) << "test can not reach here";
}

}


#undef DEFAULT_STACK_SIZE
#undef GETCONTEXT
#undef UCONTEXT_T_INIT
#undef UCONTEXT_INIT
