#ifndef _H_SINGLETION_
#define _H_SINGLETION_

#include"base_head.h"

namespace fwl{

template<class T, class X = void, int N = 0>
class Singletion
{
public:
    static T * getInstance(){
        static T v;
        return &v;
    }

};

template<class T, class X = void, int N = 0>
class SingletionPtr{
public:
    static std::shared_ptr<T> getInstance(){
        static std::shared_ptr<T> v(new T);
        return v;
    }
};

}
#endif

