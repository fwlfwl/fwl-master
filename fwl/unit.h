#ifndef __H_FWL_UNIT__
#define __H_FWL_UNIT__

/**
* @file unit.h
* @brief 文件模块封装
* @author fwl
* @email 1822363660@qq.com
* @data 2023-10-08
*/

#include<sys/stat.h>
#include"base_head.h"
#include<time.h>
#include<sys/syscall.h>
#include<malloc.h>
#include<fcntl.h>
#include"macro.h"

namespace fwl{

/**
 * @brief 打印异常堆栈信息 
 * @param[in,out] 异常堆栈信息的vector
 * @param[in] skip 跳过步数
 * @param[in] size 堆栈层数
 * */
void Backtrace(std::vector<std::string> & buf_vec, int size =  64,int skip =  1);

/**
 * @brief 异常堆栈信息转化成字符串输出
 * @param[in] skip 跳过步数
 * @param[in] size 堆栈层数
 * @param[in] priex 前缀字符串
 * */
std::string BacktraceToString(int size =  64, int skip =  2,const std::string & prefix = "");

/**
 * @brief 返回当前线程Id
 */
pid_t getThreadId();

/**
 * @brief 返回当前线程名
 * */
std::string getThreadName();

/**
 * @brief 返回当前协程名
 * */
std::string getFiberName();


/**
 * @brief 返回当前协程Id
 * */
uint32_t getFiberId();

/**
 * @文档操作类封装
 * */
class FSUnit {
public:
	/**
	* @breif 打开写文件
	* @param[in,out] ofs 文件输出流
	* @param[in,out] filename 文件名
	* @param[in] mode 文件打开模式
	*/
	static bool OpenForWrite(std::ofstream& ofs, const std::string& filename, std::ios_base::openmode mode = std::ofstream::out | std::ofstream::app);
	
	/**
	* @breif 返回文件夹路径
	* @param[in] filename 输入文件名
	* @param[out] 返回文件夹名
	*/
	static const std::string Dirname(const std::string& filename);
	
	/**
	* @breif 新建文件夹
	* @param[in] path 输入文件夹名
	* @param[out] 创建文件夹成功与否
	*/
	static bool MakeDir(const std::string& path);

	/**
	* @breif 文件信息获取
	* @param[in] path 输入文件夹名
	* @param[in] st
	* @param[out] 返回文件信息获取状态
	*/
	static int _lstat(const char * path,struct stat * st = nullptr);
	
        
    /**
    * @breif 创建文件
    * @param[in] path 输入文件夹名
    * @param[in] mode 文件夹权限
    * @param[out] 返回文件夹创建状态
    * */
    static int _mkdir(const char * path,mode_t mode = S_IRWXU | S_IRWXG | S_IXOTH | S_IROTH);
};

/***
 * @brief malloc封装类
 */
class Allocater {
public:
    /**
     * @brief 分配内存
     * @param[in] size 分配内存大小
     * @param[out] 返回指针
     * */
    static void * allocate(size_t size);
    
    /**
     * @beief 释放内存
     * */
    static void deallocate(void * ptr);


};

/**
 * @brief 文件描述符设置成非阻塞
 * */
int setnonblocking(int fd);

/**
 * @brief 获取当前时间(毫秒)
 * */
uint64_t GetTimeMs();

/**
 * @brief 获取当前时间(微秒)
 * */
uint64_t GetTimeUs();

/**
 * @brief base64 encode
 * */
std::string base64En(const char * in);


/**
 * @brief SHA1
 * */
std::string sha1(const char * in);

}
#endif // !_H_UNIT_
