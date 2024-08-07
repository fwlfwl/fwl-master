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
	* @breif 靠靠靠�
	* @param[in,out] ofs 靠�
	* @param[in,out] filename 靠�
	* @param[in] mode 靠�
	*/
	static bool OpenForWrite(std::ofstream& ofs, const std::string& filename, std::ios_base::openmode mode = std::ofstream::out | std::ofstream::app);
	
	/**
	* @breif 靠靠靠靠靠
	* @param[in] filename 靠� 
	* @param[out] 靠靠靠
	*/
	static const std::string Dirname(const std::string& filename);
	
	/**
	* @breif 靠靠�
	* @param[in] path 靠�
	* @param[out] 靠靠
	*/
	static bool MakeDir(const std::string& path);

	/**
	* @breif 靠靠
	* @param[in] path 靠靠�
	* @param[in] st 靠靠
	* @param[out] 靠靠
	*/
	static int _lstat(const char * path,struct stat * st = nullptr);
	
        
    /**
    * @breif 靠靠�
    * @param[in] path 靠靠�
    * @param[in] mode 靠靠
    * @param[out] 靠靠
    * */
    static int _mkdir(const char * path,mode_t mode = S_IRWXU | S_IRWXG | S_IXOTH | S_IROTH);

	/**
	 * @brief 靠靠靠靠�
	 * @param[in] path 靠靠
	 * @param[in] files 靠靠 
	 * @param[in] prefix 靠靠�(靠.yml)
	 * */
	static void ListAllFiles(const std::string & path, std::vector<std::string> &files, const std::string & prefix = "");
};

/***
 * @brief malloc封装类
 */
class Allocater {
public:
    /**
     * @brief 靠靠
     * @param[in] size 靠靠靠
     * @param[out] 靠靠靠靠
     * */
    static void * allocate(size_t size);
    
    /**
     * @beief 靠靠
     * */
    static void deallocate(void * ptr);


};

/**
 * @brief fd靠靠�
 * */
int setnonblocking(int fd);

/**
 * @brief 靠靠靠
 * */
uint64_t GetTimeMs();

/**
 * @brief 靠靠靠
 * */
uint64_t GetTimeUs();

/**
 * @brief base64 encode
 * */
std::string base64En(const unsigned char * in, size_t len);


/**
 * @brief SHA1
 * */
bool sha1(const std::string & in, unsigned char * out);

}
#endif // !_H_UNIT_
