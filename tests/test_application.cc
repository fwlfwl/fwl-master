#include "../fwl/system/application.h"
#include "../fwl/system/env.h"
#include "../fwl/system/daemon.h"
#include "../fwl/hook.h"
#include "../fwl/http/http.h"
#include "../fwl/http/ws_server.h"
#include "../fwl/http/ws_session.h"
#include <vector>
#include <memory>

static fwl::Logger::ptr g_logger = FWL_LOG_NAME("system");

int main(int argc, char * argv[]){
	auto env = fwl::EnvMgr::getInstance();
	if(!env){
		FWL_LOG_DEBUG(g_logger) << "Can not create env";
		return -1;
	}else{
		env -> init(argc,argv);
	}
	int cur_threadId;
	if(fwl::EnvMgr::getInstance() -> has("d") && 0 != (cur_threadId = fwl::create_background())){
		return cur_threadId;
	}
	auto app = fwl::appMgr::getInstance();
	if(!(app -> init(argc, argv))){
		return -1;
	}
	app -> run();
	return 0;	
}
