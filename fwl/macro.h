#ifndef __H_MACRO__
#define __H_MACRO__

#include"log.h"
#include"unit.h"
#include<assert.h>

#define ASSERT(exp) \
    {   \
        if(!(exp)){ \
            FWL_LOG_DEBUG(FWL_LOG_ROOT()) <<  "backtrace:" << fwl::BacktraceToString(); \
			assert(exp);	\
        }   \
    }   

#define ASSERT_PRE(exp,prefix)  \
    {   \
        if(!(exp)){ \
            FWL_LOG_DEBUG(FWL_LOG_ROOT()) << "backtrace:" <<  fwl::BacktraceToString(64,2,#prefix);\
			assert(exp);	\
        }   \
    }

#endif

