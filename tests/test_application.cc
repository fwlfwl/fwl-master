#include "../fwl/system/application.h"
#include <vector>
#include <memory>

static fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

int main(int argc, char * argv[]){
	auto app = fwl::appMgr::getInstance();
	app -> init(argc, argv);
	app -> run();
}
