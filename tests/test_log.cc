#include"../fwl/unit.h"
#include"../fwl/log.h"

int TestOpenForWrite(int argc,char * argv[]) {
    fwl::FSUnit unit;
    std::ofstream ofs;
    for(int i = 1; i < argc; ++i){
        unit.OpenForWrite(ofs,argv[i],std::ofstream::out | std::ofstream::trunc);
        ofs.close();
    }
    return 0;
}

void TestLog(){
    fwl::Logger::ptr logger(new fwl::Logger(std::string("My logger")));
    fwl::FileLogAppender::ptr fileAppender = std::make_shared<fwl::FileLogAppender>("/home/fwl/log.txt");
    fwl::StdoutLogAppender::ptr stdoutAppender = std::make_shared<fwl::StdoutLogAppender>();
    logger->addLogAppender(fileAppender);
    logger->addLogAppender(stdoutAppender);
    logger->setLogFormatter("");
    FWL_LOG_INFO(logger) << "test INFO";
    FWL_LOG_ERROR(logger) <<  "test ERROR";

    auto it = fwl::loggerMgr::getInstance() -> getLogger("XX");
    FWL_LOG_FATAL(it) << "XXX";
}

int main(){
    TestLog();
    return 0;
}
