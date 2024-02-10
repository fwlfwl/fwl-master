#ifndef __H_NONCOPY__
#define __H_NONCOPY__

/**
* @file noncopy.h
* @brief 禁止拷贝类
* @author fwl
* @email 1822363660@qq.com
* @data 2023-11-04
* */

namespace fwl{

class NonCopy
{
public:
    /**
     * @brief 禁止默认拷贝函数
     * */
    NonCopy() =  default;

    /**
     * @brief 禁止默认稀构函数
     * */
    ~NonCopy() =  default;

    /**
     * @beief 禁止拷贝构造函数
     * */
    NonCopy(const NonCopy&) =  delete;

    /**
     * @brief 禁止赋值函数
     **/
    NonCopy& operator=(const NonCopy&) = delete;

};

}
#endif
