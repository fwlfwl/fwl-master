#ifndef __H_FIBER__
#define __H_FIBER__
/**
* @file log.h
* @brief 协程模块
* @author fwl
* @email 1822363660@qq.com
* @data 2023-10-08
*/
#include<string>
#include<memory>
#include<ucontext.h>
#include<functional>

namespace fwl{

/**
 * @brief 协程状态
 * */
enum FIBER_STATE {
    //初始化
    INIT,
    //就绪
    RUNNABLE,
    //运行
    RUNNING,
    //挂起
    SUSPEND,
    //结束
    TERM,
    //超出预期
    EXPECT
};

/**
 * @brief 协程状态
 * */
class Fiber : public std::enable_shared_from_this<Fiber> {
public:
friend class Scheduler;
    typedef std::shared_ptr<Fiber> ptr;
    typedef FIBER_STATE State;

    //主协程构造函数;每个线程第一个协程
    Fiber(const std::string & name = "threadFiber");

    /**
     * @brief 子协程构造函数
     * @param[in] cb 要执行的函数
     * @param[in] size 分配执行函数的栈大小
     * @param[in] main_fiber 上下文切换主线程
     * @param[in] name 协程名
     * */
    Fiber(std::function<void()> cb , bool user_call = false, size_t size = 0, const std::string & name = "");
    
    //析构函数
    ~Fiber();
   
	//返回fiber id
	uint64_t getFiberId(){
		return m_id;
	}
    //重置，重复利用
    bool reset(std::function<void()>);

    //切回到主线成程
    void SwapOut();

    //从主协程切入当前协程
    void SwapIn();
    
    /**
     * @切换至目标协程
     * @param[in] target_fiber 目标协程序
     * */
    bool call();

    /**
     * @从目标协程切换回来
     * @param[in] target_fiber 目标协程序
     * */
    bool back();

    //设置协程状态
    void SetState(State in_state);
 
    //切换至挂起状态
    static void SwitchToSUSPEND();

    //切换至准备运行状态
    static void SwitchToRUNNABLE();

    //返回协程状态
    State GetState() const ;
public:
    //返回协程ID
    static uint32_t GetId(); 
    
    //返回协程名
    static std::string GetName();

    //设置当前协程
    static void SetThis(Fiber *);

    //获取当前协程
    static std::shared_ptr<Fiber> GetThis();

    //运行主函数
    static void MainRun();
    
    //运行调度器主函数
    static void SchedulerMainRun();
private:
    //协程ID
    uint64_t m_id = 0;
    //协程NAME
    std::string m_name;
    //uncontext结构
    ucontext_t m_ctx;
    //栈空间
    void * m_stack = nullptr;
    //栈空间大小
    size_t m_stacksize =  0;
    //Fiber状态
    State m_state = INIT;
    //执行函数
    std::function<void()> m_cb = nullptr;
};

}
#endif


