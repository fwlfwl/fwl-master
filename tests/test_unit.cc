#include "../fwl/macro.h"

void is_assert(){
    ASSERT_PRE(0 ==  1,test);
    //ASSERT(0 ==  1);
}

void test_assert(){
    is_assert();
}

int main()
{
    test_assert();
    return 0;
}

