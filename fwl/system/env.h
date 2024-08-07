#ifndef __H_ENV__
#define __H_ENV__ 
/**
 * @brief env.h
 * @param[in] 环境变量信息 
 * */

#include <map>
#include <string>
#include "../mutex.h"
#include "../singletion.h"

namespace fwl{

/**
 * @brief env类
 * */
class Env{
public:
	typedef RWMutex MutexType;

	/**
	 * @brief 初始化函数
	 * */
	bool init(int argc, char * argv[]);

	/**
	 * @brief args操作函数
	 * */
	std::string get(const std::string & key, const std::string & defaule_value);
	void add(const std::string & key, const std::string & val);
	bool has(const std::string & key);
	void del(const std::string & key);
	
	/**
	 * @brief helps操作函数
	 * */
	std::string getHelp(const std::string & key, const std::string & default_val);
	void addHelp(const std::string & key, const std::string & val);
	bool hasHelp(const std::string & key);
	void delHelp(const std::string & key);
	void printHelps();

	/**
	 * @brief 返回路径
	 * */
	const std::string & getExe() const{
		return m_exe;
	}

	const std::string & getCwd() const{
		return m_cwd;
	}
	
	//获取完整的运行路径(即exe路径，config文件存在此目录，作为config默认目录)
	void getExePath(std::string & out) const;
private:
	//参数
	std::map<std::string, std::string> m_args;
	//说明
	std::map<std::string, std::string> m_helps;
	//程序名 
	std::string m_program;
	//获取/proc/pid/exe路径
	std::string m_exe;
	//获取/proc/pid/cwd路径
	std::string m_cwd;
	//读写锁 
	RWMutex m_mutex;		
};

typedef Singletion<Env> EnvMgr;

}
#endif
