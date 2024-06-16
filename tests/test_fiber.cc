#include "../fwl/fiber.h"
#include"../fwl/log.h"

static fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

void f_fiber1(){
    FWL_LOG_INFO(g_logger) << "fiber1 begin";
    std::shared_ptr<fwl::Fiber> m_fiber = fwl::Fiber::GetThis();
    
    FWL_LOG_INFO(g_logger) << "fiber1 back";
    m_fiber -> back();
    FWL_LOG_INFO(g_logger) << "fiber1 call";
    FWL_LOG_INFO(g_logger) << "fiber1 end";
}


void test_fiber(){
	fwl::Fiber::GetThis();
	FWL_LOG_INFO(g_logger) << "main fiber begin";
    std::shared_ptr<fwl::Fiber> fiber1(new fwl::Fiber(std::function<void()>(f_fiber1)));
    
    FWL_LOG_INFO(g_logger) << "fiber1 call()";
    fiber1 -> call();
	FWL_LOG_INFO(g_logger) << "main fiber call";
	fiber1 -> call();
    FWL_LOG_INFO(g_logger) << "main fiber end";
}

int main()
{
    test_fiber();
    return 0;
}
