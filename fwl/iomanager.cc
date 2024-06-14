#include<sys/epoll.h>
#include<unistd.h>
#include<iostream>
#include"iomanager.h"
#include"macro.h"
#include"log.h"

#define MAX_EVENT_SIZE 1024
#define DEFAULT_FD_SIZE 64
#define TICK_MSG "T"

namespace fwl{

static Logger::ptr g_logger = FWL_LOG_NAME("system");

//IOManager::FdContext类实现

/**
 * @brief 获取事件上下文
 * @param[in] 事件类-READ/WRITE
 * */
IOManager::FdContext::EventContext & IOManager::FdContext::GetContext(Event event){
    switch(event){
        case READ:{
            return m_readCtx;
        }
        case WRITE:{
            return m_writeCtx;
        }
        default:
            break;
    }
      
    ASSERT_PRE(false,"event is illegal!");
}

//重置事件
void IOManager::FdContext::ResetContext(EventContext & etx){
    etx.m_sc = nullptr;
    etx.m_fiber.reset();
    etx.m_cb = nullptr;
}

/**
* @brief 重置当前读写事件
* */
void IOManager::FdContext::ResetContext(){
    ResetContext(m_readCtx);
    ResetContext(m_writeCtx);
    m_event = NONE;
}

/**
* @brief 触发事件
* @param[in] 输入事件类型-READ/WRITE 
*/
void IOManager::FdContext::TriggerContext(Event event){
    if(m_event & event){
        EventContext & ectx = GetContext(event);
        if(ectx.m_cb){
            ectx.m_sc -> scheduler(ectx.m_cb);
            return;
        }else if(ectx.m_fiber){
			//FWL_LOG_DEBUG(g_logger) << "current fiber" << fwl::Fiber::GetThis();
			FWL_LOG_DEBUG(g_logger) << "switch fiber id:" << ectx.m_fiber -> getFiberId();
            ectx.m_sc -> scheduler(ectx.m_fiber);
            return;
        }else{
            FWL_LOG_ERROR(g_logger) << "TriggerContext() error : m_cb & m_fiber is NULL";
        }
    }
    FWL_LOG_ERROR(g_logger) <<  "event " << event << " is illegal,current event is " << m_event;
    return;
}


//IOManager类实现

//构造函数
IOManager::IOManager(size_t threadNum,bool use_call, const::std::string & name)
    :Scheduler(threadNum, use_call, name){
    //创建内核事件表
    m_epollfd = epoll_create(MAX_EVENT_SIZE);
    ASSERT(0 <=  m_epollfd);
    
    //创建管道
    int ret = pipe(m_tickPipefd);
    ASSERT(0 == ret);

    //添加事件
    setnonblocking(m_tickPipefd[0]);
	opfd(m_epollfd, m_tickPipefd[0], EPOLL_CTL_ADD, EPOLLIN);
   	 
    //重新分配大小
    resize(DEFAULT_FD_SIZE);

    //开始运行
    start();
}

//析构函数
IOManager::~IOManager(){
    //停止运行
    stop();
    //关闭所有已经打开的句柄
    closeAll();
}

/**
* @brief epoll事件操作
* @param[in] epollfd epoll句柄
* @param[in] fd 事件句柄
* @param[in] op epoll操作
* @param[in] enable_et ET操作是否允许
* */
void IOManager::opfd(int epollfd, int fd, int op, int eType, bool enable_et){
    struct epoll_event event;
    memset(&event,0,sizeof(event));
    event.events = eType;
    //FWL_LOG_DEBUG(g_logger) << event.events;
    event.data.fd = fd;
    if(enable_et){
        event.events |= EPOLLET;
    }
    
    int ret = epoll_ctl(epollfd, op, fd, &event);
    //setnonblocking(fd);
    if(ret){
        FWL_LOG_ERROR(g_logger) << "epoll_ctl(" << epollfd << "," << fd << ","<< op
            << "," << eType << "),error:"<< errno << ",strerror(errno):" << strerror(errno)
            << " " << event.events;
    }
}
    
/**
* @brief 重新分配事件器大小
* */
void IOManager::resize(size_t size){
    if(0 < size){
        m_fdContexts.resize(size);
        for(size_t i =  0; i < size; ++i){
            try{
                m_fdContexts[i] = new FdContext;
                m_fdContexts[i] -> m_fd = i;
            }catch(std::exception & e){
                FWL_LOG_ERROR(g_logger) << "FdContext alloc failed,error is " << e.what();
                ASSERT(false);
            }catch(...){
                FWL_LOG_ERROR(g_logger) << "FdContext alloc failed";
            }
       }
    }
}

/**
* @brief 关闭打开已打开句柄
* */
void IOManager::closeAll(){
    close(m_epollfd);
    close(m_tickPipefd[1]);
    close(m_tickPipefd[0]);
    IOManager::RWMutexType::WriteLock lock(m_mutex);
    for(int i = 0; i < (int)m_fdContexts.size(); ++i){
        if(m_fdContexts[i]){
            delete m_fdContexts[i];
        }
    }
}

/**
* @brief 添加事件
* @param[in] fd 添加句柄
* @param[in] event 事件类型-读/写
* @param[in] cb 执行函数
* */
int IOManager::addEvent(int fd, Event event, std::function<void()> cb){
    FdContext * new_fd_ctx;
    //判断是否需要扩容
    IOManager::RWMutexType::ReadLock rlock(m_mutex);
    if(fd >= (int)m_fdContexts.size()){
        rlock.unlock();
        IOManager::RWMutexType::WriteLock wlock(m_mutex);
        resize((size_t)fd * 1.5);
        new_fd_ctx = m_fdContexts[fd];  
        wlock.unlock();
    }else{
        new_fd_ctx = m_fdContexts[fd];  
        rlock.unlock();
    }

    FdContext::MutexType::Lock lock(new_fd_ctx -> m_mutex);   
    
    int op =  new_fd_ctx -> m_event ? EPOLL_CTL_MOD : EPOLL_CTL_ADD;
    opfd(m_epollfd, fd, op, event);

    ++m_ready_eventNum;
    int new_event = NONE;
    new_event = new_fd_ctx -> m_event | event;
    new_fd_ctx -> m_event = (Event)new_event;
    
    //设置EventContext
    FdContext::EventContext& ectx = new_fd_ctx -> GetContext(event);
    ectx.m_sc = Scheduler::GetThis();
    if(cb){
        ectx.m_cb.swap(cb);
    }else{
        ectx.m_fiber = Fiber::GetThis();
    }
	//FWL_LOG_DEBUG(g_logger) << "fd " << new_fd_ctx -> m_fd << 
     //   " add event " << event << ",current event " << new_fd_ctx -> m_event;
	return 0;
}
    
/**
* @brief 删除事件
* @param[in] fd 删除句柄
* @param[in] event 事件类型-读/写
* */
bool IOManager::delEvent(int fd, Event event){
    //取事件
    IOManager::RWMutexType::ReadLock lock(m_mutex);
    if(fd >= (int)m_fdContexts.size()){
        return false;
    }
    FdContext * fd_ctx = m_fdContexts[fd];
    lock.unlock();
 
    FdContext::MutexType::Lock lock2(fd_ctx -> m_mutex);
    //判断删除后是否有未处理的事件
    int new_event = fd_ctx -> m_event & ~event;
    int op = (new_event ? EPOLL_CTL_MOD : EPOLL_CTL_DEL);
    opfd(m_epollfd, fd, op, new_event);    
    
    --m_ready_eventNum;
    fd_ctx -> m_event = (Event)new_event;
    FdContext::EventContext & ectx = fd_ctx -> GetContext(event);
    fd_ctx -> ResetContext(ectx);
    return true;
}

/**
* @brief 取消事件
* @param[in] fd 句柄
* @param[in] event 事件类型-读/写
* */
bool IOManager::cancelEvent(int fd,Event event){
    IOManager::RWMutexType::ReadLock lock(m_mutex);
    if(fd >= (int)m_fdContexts.size()){
        return false;
    }
    FdContext * fd_ctx = m_fdContexts[fd];
    lock.unlock();

    IOManager::FdContext::MutexType::Lock lock2(fd_ctx -> m_mutex);
    //判断当前事件是否存在
    if(0 == (event & fd_ctx -> m_event)){
        FWL_LOG_INFO(g_logger) << "fd " << fd << " not exist event:" << event
            << ",current event:" << fd_ctx -> m_event;
        return false;
    }

    int new_event = fd_ctx -> m_event & ~event;
    int op = new_event ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;
    opfd(m_epollfd, fd, op, new_event);

    FWL_LOG_DEBUG(g_logger) << "fd " << fd << " del event " << event
        << ",new event is " << new_event; 

    --m_ready_eventNum;
    //执行事件任务
    fd_ctx -> TriggerContext(event);
    //删除事件
    IOManager::FdContext::EventContext& ectx = fd_ctx -> GetContext(event);
    fd_ctx -> ResetContext(ectx);
    fd_ctx -> m_event = (Event)new_event;
    return true;
}

/**
* @brief 取消所有事件
* */
bool IOManager::cancelAll(int fd){
    IOManager::RWMutexType::ReadLock lock(m_mutex);
    if(fd >= (int)m_fdContexts.size()){
        return false;
    }
    FdContext * fd_ctx = m_fdContexts[fd];
    lock.unlock();

    IOManager::FdContext::MutexType::Lock lock2(fd_ctx -> m_mutex);
    //存在事件则先触发
    if(fd_ctx -> m_event & READ){
        --m_ready_eventNum;
       fd_ctx -> TriggerContext(READ);       
    }
    if(fd_ctx -> m_event & WRITE){
        --m_ready_eventNum;
        fd_ctx -> TriggerContext(WRITE);
    }
    fd_ctx -> ResetContext();
    return true;
}

/**
* @brief 返回当前IOManager
* */
IOManager * IOManager::GetThis(){
    return dynamic_cast<IOManager *>(Scheduler::GetThis());
}

/**
* @brief通知调度函数
*/
void IOManager::tick(){
	int ret = write(m_tickPipefd[1], TICK_MSG, sizeof(TICK_MSG));
    ASSERT(ret == sizeof(TICK_MSG));
    //FWL_LOG_DEBUG(g_logger) <<  sizeof(TICK_MSG) << ",write msg is " << TICK_MSG;
}

/**
* @berief stopping函数
* */
bool IOManager::stopping(){
    uint64_t timeout =  0;
    return stopping(timeout);
}

/**
 * @brief 带定时器的stopping
 * */
bool IOManager::stopping(uint64_t & timeout){
    timeout = getNextTimer();
    return 0 == m_ready_eventNum && ~0ul == timeout && Scheduler::stopping();
}

/**
* @brief stopping函数
* @param[in] timeout 超时时间
* */
void IOManager::onFrontEvent(){
	tick();
}

/**
* @berief 无任务运行函数
* */
void IOManager::idle(){
    
    FWL_LOG_INFO(g_logger) << "idle..";
    epoll_event * events = new epoll_event[MAX_EVENT_SIZE]();
    std::shared_ptr<epoll_event> shared_events(events,[](epoll_event * p){
                                                delete[] p;
                                               });
    int ret = 0;
    uint64_t timeout = ~0ul;
    while(true){    
        if(stopping(timeout)){
            FWL_LOG_DEBUG(g_logger) << "exit";
            break;
        }
        //循环wait，有事件后退出
		do{
            static uint64_t DEFAULT_WAIT_TIME = 3000;
            uint64_t wait_time = timeout < DEFAULT_WAIT_TIME ? timeout : DEFAULT_WAIT_TIME;
            ret = epoll_wait(m_epollfd, events, MAX_EVENT_SIZE, (int)wait_time);
            FWL_LOG_DEBUG(g_logger) << "epoll_wait,ret = " << ret;
			if(0 > ret && errno == EINTR){
            }else{
                break;
            }
        }while(1);
        //处理定时器
		std::vector<std::function<void()> > cbs;
        handleExpireTimer(cbs);
        for(auto it : cbs){
			scheduler(it);
        }

        //处理epoll事件
        for(int i =  0; i < ret; ++i){
            epoll_event& event = events[i];
            //FWL_LOG_DEBUG(g_logger) << "Run fd:" << event.data.fd << ",pipefd:" << m_tickPipefd[0];
			//处理ticket信号
            if(event.data.fd == m_tickPipefd[0]){
                char msg[1024];
                while(read(m_tickPipefd[0],&msg,sizeof(msg)) >  0){
                    //FWL_LOG_DEBUG(g_logger) <<  sizeof(msg) << ",msg is " << msg << ",ret is " << ret;
                }
                continue;
            }
            //记录事件类型
            int real_event = NONE;
            if(event.events & READ){
                real_event |= READ;
            }
            if(event.events & WRITE){
                real_event |= WRITE;
            }
            
            //执行事件
            if(real_event & READ){
                cancelEvent(event.data.fd, READ);
            }
            if(real_event & WRITE){
                cancelEvent(event.data.fd, WRITE);
            }
		}
        Fiber::ptr cur_fiber = Fiber::GetThis();
        auto cur_ptr = cur_fiber.get();
        cur_fiber.reset();
        cur_ptr -> SwapOut();
    }
}

}
