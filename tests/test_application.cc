#include "../fwl/system/application.h"
#include "../fwl/system/env.h"
#include "../fwl/system/daemon.h"
#include <vector>
#include <memory>

static fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

int main(int argc, char * argv[]){
	auto app = fwl::appMgr::getInstance();
	if(fwl::EnvMgr::getInstance() -> has("d") && 0 != fwl::create_background()){
		goto exitPos;
	}
	app -> init(argc, argv);
	app -> run();
exitPos:
	FWL_LOG_INFO(g_logger) << "Thread " << getpid() << " exited!";
	return 0;
}
