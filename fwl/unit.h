#ifndef __H_FWL_UNIT__
#define __H_FWL_UNIT__

/**
* @file unit.h
* @brief �ļ�ģ���װ
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
 * @brief ��ӡ�쳣��ջ��Ϣ 
 * @param[in,out] �쳣��ջ��Ϣ��vector
 * @param[in] skip ��������
 * @param[in] size ��ջ����
 * */
void Backtrace(std::vector<std::string> & buf_vec, int size =  64,int skip =  1);

/**
 * @brief �쳣��ջ��Ϣת�����ַ������
 * @param[in] skip ��������
 * @param[in] size ��ջ����
 * @param[in] priex ǰ׺�ַ���
 * */
std::string BacktraceToString(int size =  64, int skip =  2,const std::string & prefix = "");

/**
 * @brief ���ص�ǰ�߳�Id
 */
pid_t getThreadId();

/**
 * @brief ���ص�ǰ�߳���
 * */
std::string getThreadName();

/**
 * @brief ���ص�ǰЭ����
 * */
std::string getFiberName();


/**
 * @brief ���ص�ǰЭ��Id
 * */
uint32_t getFiberId();

/**
 * @�ĵ��������װ
 * */
class FSUnit {
public:
	/**
	* @breif ��д�ļ�
	* @param[in,out] ofs �ļ������
	* @param[in,out] filename �ļ���
	* @param[in] mode �ļ���ģʽ
	*/
	static bool OpenForWrite(std::ofstream& ofs, const std::string& filename, std::ios_base::openmode mode = std::ofstream::out | std::ofstream::app);
	
	/**
	* @breif �����ļ���·��
	* @param[in] filename �����ļ���
	* @param[out] �����ļ�����
	*/
	static const std::string Dirname(const std::string& filename);
	
	/**
	* @breif �½��ļ���
	* @param[in] path �����ļ�����
	* @param[out] �����ļ��гɹ����
	*/
	static bool MakeDir(const std::string& path);

	/**
	* @breif �ļ���Ϣ��ȡ
	* @param[in] path �����ļ�����
	* @param[in] st
	* @param[out] �����ļ���Ϣ��ȡ״̬
	*/
	static int _lstat(const char * path,struct stat * st = nullptr);
	
        
    /**
    * @breif �����ļ�
    * @param[in] path �����ļ�����
    * @param[in] mode �ļ���Ȩ��
    * @param[out] �����ļ��д���״̬
    * */
    static int _mkdir(const char * path,mode_t mode = S_IRWXU | S_IRWXG | S_IXOTH | S_IROTH);
};

/***
 * @brief malloc��װ��
 */
class Allocater {
public:
    /**
     * @brief �����ڴ�
     * @param[in] size �����ڴ��С
     * @param[out] ����ָ��
     * */
    static void * allocate(size_t size);
    
    /**
     * @beief �ͷ��ڴ�
     * */
    static void deallocate(void * ptr);


};

/**
 * @brief �ļ����������óɷ�����
 * */
int setnonblocking(int fd);

/**
 * @brief ��ȡ��ǰʱ��(����)
 * */
uint64_t GetTimeMs();

/**
 * @brief ��ȡ��ǰʱ��(΢��)
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
