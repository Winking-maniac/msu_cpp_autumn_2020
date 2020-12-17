#include "pool.h"
#include <iostream>

struct A {
    int value;
    A(int value) : value(value) {;};
};

int foo(const A& x) {
    return x.value;
}


int
main()
{
    int tests_failed = 0;
    
    ThreadPool pool(8);
    
    // test 1
    auto task1 = pool.exec(foo, A(8));
    if (task1.get() != 8) {
        tests_failed++;
        std::cout << "First test failed" << std::endl;
    }

    // test 2
    auto task2 = pool.exec([]() { return 1; });
    
    if (task2.get() != 1) {
        tests_failed++;
        std::cout << "Second test failed" << std::endl;
    }
    
    
    if (tests_failed) {
        std::cout << std::endl << tests_failed << " of 2 test failed" << std::endl;
    } else {
        std::cout << std::endl << "All tests passed" << std::endl;
    }
}
