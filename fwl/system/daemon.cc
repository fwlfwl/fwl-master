#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include "daemon.h"
#include "../log.h"
#include "../config.h"

namespace fwl{

static const uint32_t DEFAULE_RESTART_TIME = 5;

static Logger::ptr g_logger = FWL_LOG_NAME("system");
static ConfigVar<uint32_t>::ptr g_restart_config = Config::lookUp("daemon.resetartTime",DEFAULE_RESTART_TIME, "Restart time when encountering errors");

/**
 * @brief 守护进程实际执行函数
 * */
static int real_daemon(int argc, char * argv[], std::function<int(int argc, char* argv[])> main_fun){
	ProcessInfoType::getInstance() -> parent_id = getpid();
	ProcessInfoType::getInstance() -> parent_runtime = time(0);
	pid_t pid;
	while(true){
		if(0 > (pid = fork())){
			FWL_LOG_ERROR(g_logger) << "Create daemon failed, error=" << errno << ", errorstr="<<strerror(errno);
			return -1;
		}else if(0 == pid){	//子进程
			ProcessInfoType::getInstance() -> main_runtime = time(0); 
			ProcessInfoType::getInstance() -> main_id = getpid();		
			return main_fun(argc,argv);	
		}else{	//父进程-等待子进程退出（无其他功能，使用wait）
			FWL_LOG_INFO(g_logger) << "Daemon start";
			int status = 0;
			waitpid(pid, &status, 0);
			FWL_LOG_INFO(g_logger) << (ProcessInfoType::getInstance() -> toString());
			//判断返回状态
			if(WIFEXITED(status)){	//正常退出，不重拉
				FWL_LOG_INFO(g_logger) << "Child " << pid << " exit!";
				break;
			}else{	//异常退出
				++ProcessInfoType::getInstance() -> resetart_num;	
				sleep(g_restart_config -> getValue());
			}
		}
	}
	return 0;
}

/**
 * @brief 主运行函数 
 * @param[in] argc 参数个数
 * @param[in] argv 参数内容
 * @param[in] main_fun 主运行函数 
 * @param[in] is_daemon 是否守护函数运行
 * @param[out] int 返回整形结果
 * */
int start_daemon(int argc, char * argv[], std::function<int(int argc, char * argv[])> main_fun,bool is_daemon){
	if(is_daemon){
		return real_daemon(argc, argv, main_fun);
	}
	return main_fun(argc, argv);
}

/**
 * @brief 创建后台进程
*/ 
int create_background(){
	//创建子进程
	pid_t pid = fork();
	if(0 > pid){
		FWL_LOG_ERROR(g_logger) << "Create backgroun daemon error, error=" << errno << ", errorstr="<<strerror(errno);
;
		exit(-1);
	}else if(0 < pid){	
		//主进程直接退出
		exit(1);
	}
	if(setsid() < 0){
		FWL_LOG_ERROR(g_logger) << "Create session failed, error=" << errno << ", errorstr="<<strerror(errno);
		exit(-1);
	}	
	umask(0);
	int fd = open("/dev/null",O_RDWR);
	if(0 > fd){
		FWL_LOG_ERROR(g_logger) << "Open /dev/null failed, error="<< errno <<  ", errorstr=" << strerror(errno);
		exit(-1);
	}	
	if(0 > dup2(fd, STDIN_FILENO)){
		FWL_LOG_ERROR(g_logger) << "Dup stdin failed, error="<< errno <<  ", errorstr=" << strerror(errno);
	}
	if(0 > dup2(fd, STDOUT_FILENO)){
		FWL_LOG_ERROR(g_logger) << "Dup stdout failed, error="<< errno <<  ", errorstr=" << strerror(errno);
	}
	close(fd);
	return 0;	//子进程退出
}

}

