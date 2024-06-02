#include "env.h"
#include "../log.h"
#include <iostream>
#include <iomanip>
#include <unistd.h>
namespace fwl{

static Logger::ptr g_logger = FWL_LOG_NAME("system");

/**
 * @brief 初始化函数
 * @param[in] argc 输入参数个数(包括开头程序名)
 * @param[in] argv 参数内容
 * */
bool Env::init(int argc, char * argv[]){
	char path[1024];
	char link[1024];
	ssize_t path_len = 0;
	sprintf(link, "/proc/%d/cwd",getpid());
	path_len = readlink(link, path, sizeof(path));
	if(0 >= path_len){
		FWL_LOG_ERROR(g_logger) << "cwd read fail";
		return false;
	}
	m_cwd = path;
	path_len = 0;

	bzero(link, sizeof(link));
	bzero(path, sizeof(path));
	sprintf(link, "/proc/%d/exe", getpid());
	path_len = readlink(link, path, sizeof(path));	
	if(0 >= path_len){
		FWL_LOG_ERROR(g_logger) << "exe read fail";
		return false;
	}
	m_exe = path;

	char * key = nullptr;
	for(int i = 1; i < argc; ++i){
		if('-' == argv[i][0]){
			if(strlen(argv[i]) > 1){
				key = &argv[i][1];
			}else{
				FWL_LOG_ERROR(g_logger) << "Invalid args index " << i;
				return false;
			}
		}
		if(nullptr == key){
			FWL_LOG_ERROR(g_logger) << "Invalid args index " << i;
			return false;
		}
		if(i + 1 < argc && '-' != argv[i + 1][0]){
			add(key, argv[i + 1]);
			++i;
		}else{
			add(key, "");
		}
		key = nullptr;
	}
	assert(argv[0]);
	m_program = argv[0];
	return true;
}

/**
 * @brief args操作函数
 * */
std::string Env::get(const std::string & key, const std::string & default_val){
	MutexType::ReadLock lock(m_mutex);
	auto str_it = m_args.find(key);
	return str_it == m_args.end() ? default_val : str_it -> second; 
}
void Env::add(const std::string & key, const std::string & val){
	MutexType::WriteLock lock(m_mutex);
	m_args[key] = val;
}

bool Env::has(const std::string & key){
	MutexType::ReadLock lock(m_mutex);
	return m_args.find(key) != m_args.end();
}

void Env::del(const std::string & key){
	MutexType::WriteLock lock(m_mutex);
	m_args.erase(key);
}

/**
 * @brief helps操作函数
 * */
std::string Env::getHelp(const std::string & key,const std::string & defaule_val){
	MutexType::ReadLock lock(m_mutex);
	auto str_it = m_helps.find(key);
	return str_it == m_args.end() ? defaule_val : str_it -> second; 
}

void Env::addHelp(const std::string & key, const std::string & val){
	MutexType::WriteLock lock(m_mutex);
	m_helps[key] = val;	
}

bool Env::hasHelp(const std::string & key){
	MutexType::ReadLock lock(m_mutex);
	return m_helps.find(key) != m_helps.end(); 
}

void Env::delHelp(const std::string & key){
	MutexType::WriteLock lock(m_mutex);
	m_helps.erase(key);	
}

void Env::printHelps(){
	MutexType::ReadLock lock(m_mutex);
	std::cout << "Usage:" << "[option] " << "[describe]"<< std::endl;
	for(auto & help : m_helps){
		std::cout << std::setw(5) << help.first << ":" << help.second << std::endl;
	}	
}
}
