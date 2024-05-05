#include"unit.h"
#include<stdlib.h>
#include<execinfo.h>
#include<sys/time.h>
#include"fiber.h"
#include"thread.h"
#include"log.h"

#include<fstream>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/crypto.h>

namespace fwl{

static Logger::ptr g_logger = FWL_LOG_NAME("system");

void Backtrace(std::vector<std::string> & vec_buf, int size, int skip){
    void** buf = (void **)malloc(size * sizeof(void*));
    //判断malloc是否成功
    if(!buf){
        FWL_LOG_ERROR(g_logger) << "backtrace error";
        free(buf);
        return;
    } 
    
    //返回实际函数嵌套层数
    int true_size = backtrace(buf,size);

    //返回实际堆栈信息
    char ** strings = backtrace_symbols(buf,size);
    if(!strings){
        FWL_LOG_ERROR(g_logger) << "backtrace_symbols error";
        free(buf);
        return;
    }

    for(int i =  skip; i < true_size; ++i){
        vec_buf.push_back(std::string(strings[i]));
    }

    //释放内存
    free(strings);
    free(buf);
}

std::string BacktraceToString(int size, int skip, const std::string & prefix ){
    if(skip >= size){
        return std::string("");
    }
    
    std::vector<std::string> vec;
    Backtrace(vec,size,skip);
    
    if(vec.empty()){
        return std::string("");
    }

    std::stringstream ss;
    for(auto it : vec){
        ss << prefix << it << std::endl;
    }
    return ss.str();
}

pid_t getThreadId(){
    return syscall(SYS_gettid);
} 

uint32_t getFiberId(){
    return fwl::Fiber::GetId();
}

/**
 * @brief 返回当前线程名
 * */
std::string getThreadName(){
    return fwl::Thread::GetName();   
};

/**
 * @brief 返回当前协程名
 * */
std::string getFiberName(){
    return fwl::Fiber::GetName();
}



bool FSUnit::OpenForWrite(std::ofstream& ofs, const std::string& filename, std::ios_base::openmode mode){
    if (!ofs.is_open()) {
		std::string dirname = Dirname(filename);
	    MakeDir(dirname);
        ofs.open(filename,mode);
    }
    return ofs.is_open();
}


const std::string FSUnit::Dirname(const std::string& filename) {
	if (filename.empty()) {
		return ".";
	}
	size_t pos = filename.rfind('/');
	if (0 == pos) {
		return "/";
	}
	else if (std::string::npos == pos) {
		return ".";
	}
	else {
		return filename.substr(0,pos);
	}

}

bool FSUnit::MakeDir(const std::string& dirname) {
	if(0 == _lstat(dirname.c_str())){
        return true;
    }
    auto len = dirname.size();
    char path[len + 1];
    strncpy(path,dirname.c_str(),len);
    path[len] = '\0';
    char * ptr = strchr(path + 1, '/');
    if(ptr){
        for(;ptr;*ptr = '/',ptr = strchr(ptr + 1, '/')){
            *ptr = '\0';
            std::cout << path << std::endl;
            if(0 != _mkdir(path) && 0 != _lstat(path))
                return false;
        }
    }
    if(!ptr && 0 == _mkdir(path)){
        std::cout << path << std::endl;
        return true;
    }
    return false;
}

int FSUnit::_lstat(const char * path, struct stat* st) {
	struct stat tmp_st;
	int ret = lstat(path,&tmp_st);
	if (st) {
		*st = tmp_st;
	}
	return ret;
}

int FSUnit::_mkdir(const char * path,mode_t mode) {
	if(0 == access(path,W_OK))
        return 0;
    return mkdir(path,mode);

}

//Allocater实现
void * Allocater::allocate(size_t size){
    void * pointer = nullptr;
    if(0 < size){
        pointer = (void *)malloc(size);
    }
    ASSERT_PRE(nullptr != pointer,"Fiber malloc!");
    return pointer;
}

void Allocater::deallocate(void * ptr){
    if(ptr){
        free(ptr);
    }
    ptr = nullptr;
}

/**
 * @brief 文件描述符设置成非阻塞
 * */
int setnonblocking(int fd){
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

/**
 * @brief 获取当前时间(毫秒)
 * */
uint64_t GetTimeMs(){
    struct timeval tm;
    gettimeofday(&tm, 0);
    return tm.tv_sec * 1000ul + tm.tv_usec / 1000;
}

/**
 * @brief 获取当前时间(微秒)
 * */
uint64_t GetTimeUs(){
    struct timeval tm;
    gettimeofday(&tm, 0);
    return tm.tv_sec * 1000 * 1000ul + tm.tv_usec;
}

/**
 * @brief base64 encode
 * */
std::string base64En(const unsigned char * in, size_t len){
	BIO *bio = NULL, *b64 = NULL;
	BUF_MEM * bptr;
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bio);
	BIO_write(b64, in, len);
	BIO_flush(b64);
	BIO_get_mem_ptr(b64, &bptr);
	
	std::string out;
	out.resize(bptr -> length - 1);
	//delete last '\0'
	memcpy(&out[0], bptr -> data, bptr -> length - 1);
	BIO_free_all(b64);
	return out;
}


/**
 * @brief SHA1
 * */
bool sha1(const std::string & in, unsigned char * out){
	SHA1((const unsigned char *)&in[0], in.size(), out);
	return true;
}
}
