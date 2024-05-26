#ifndef __H_DAEMON__
#define __H_DAEMON__

#include <functional>
#include "../singletion.h"
namespace fwl{

/**
 * @brief 进程信息
 * */
struct ProcessInfo{
	pid_t parent_id;	//父进程信息
	pid_t main_id;	//主进程信息
	uint64_t parent_runtime;	//父进程运行时间
	uint64_t main_runtime;	//子进程运行时间
	int resetart_num;	//重启次数
	std::string toString() const{
		std::stringstream ss;
		ss << "[parent_id:" << parent_id 
			<<"],[parent_start_runtime:" << parent_runtime
			<<"],[main_id:" << main_id
			<<"],[main_start_runtime:" << main_runtime 
			<<"],[resetart_num:" << resetart_num 
			<<"]" << std::endl;
		return ss.str();
	}
};

//进程单例
typedef Singletion<ProcessInfo> ProcessInfoType;

/**
 * @brief 主运行函数 
 * @param[in] argc 参数个数
 * @param[in] argv 参数内容
 * @param[in] main_fun 主运行函数 
 * @param[in] is_daemon 是否守护函数运行
 * @param[out] int 返回整形结果
 * */
int start_daemon(int argc, char * argv[], std::function<int(int argc, char * argv[])> main_fun,bool is_daemon);

/**
 * @brief 创建后台进程
*/ 
int create_background();
}


#endif
