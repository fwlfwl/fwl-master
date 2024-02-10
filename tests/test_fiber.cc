#include "../fwl/fiber.h"
#include"../fwl/log.h"

static fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

void f_fiber1(){
    FWL_LOG_INFO(g_logger) << "fiber1 begin";
    std::shared_ptr<fwl::Fiber> m_fiber = fwl::Fiber::GetThis();
    
    //FWL_LOG_INFO(g_logger) << "fiber1 SwapOut";
    // m_fiber -> SwapOut();
    
    FWL_LOG_INFO(g_logger) << "fiber1 end";
}

static fwl::Fiber::ptr main_fiber(new fwl::Fiber);

void test_fiber(){
    FWL_LOG_INFO(g_logger) << "main fiber begin";
    std::shared_ptr<fwl::Fiber> fiber1(new fwl::Fiber(main_fiber.get(),std::function<void()>(f_fiber1)));
    
    FWL_LOG_INFO(g_logger) << "fiber1 SwapIn";
    fiber1 -> SwapIn();

    FWL_LOG_INFO(g_logger) << "main fiber end";
}

int main()
{
    test_fiber();
    return 0;
}
