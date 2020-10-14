#include "allocator.h"
#include <iostream>

enum
{
    SIZE = 10000,
    NUM_ALLOCS = 5,
    INVALID_SIZE = -100,
};

int
big_alloc()
{
    try 
    {
        Allocator x;
        x.makeAllocator(SIZE);
        char *ptr = x.alloc(2 * SIZE);
        if (ptr == nullptr) {
            std::cout << "Big alloc test - correct" << std::endl;
            return 0;
        } else {
            std::cout << "Big alloc test - failed" << std::endl;
            return 1;
        }
    }
    catch(std::exception err)
    {
        std::cout << "Big alloc test - failed" << std::endl;
        return 1;
    }
    
}

int
many_allocs()
{
    try
    {
        Allocator x;
        x.makeAllocator(SIZE);
        char *ptr;
        for (int i = 0; i < NUM_ALLOCS; i++) {
            ptr = x.alloc(SIZE / NUM_ALLOCS);
            if (ptr == nullptr) {
                std::cout << "Many allocs test - failed" << std::endl;
                return 1;
            }
        }
        std::cout << "Many allocs test - correct" << std::endl;
        return 0;
    }
    catch(std::exception err)
    {
        std::cout << "Many allocs test - failed" << std::endl;
        return 1;
    }
    
}

int
reset_alloc()
{
    try
    {
        Allocator x;
        x.makeAllocator(SIZE);
        char *ptr, *cur_ptr;
        for (int i = 0; i < NUM_ALLOCS; i++) {
            cur_ptr = x.alloc(SIZE / NUM_ALLOCS);
            if (!i) {
                ptr = cur_ptr;
            }
            if (cur_ptr == nullptr || ptr != cur_ptr) {
                std::cout << "Many allocs test - failed" << std::endl;
                return 1;
            }
            x.reset();
        }
        std::cout << "Many allocs test - correct" << std::endl;
        return 0;
    }
    catch(std::exception err)
    {
        std::cout << "Many allocs test - failed" << std::endl;
        return 1;
    }
}

int
uninitialized_alloc()
{
    try
    {
        Allocator x;
        x.alloc(SIZE);
    }
    catch (std::logic_error err) 
    {
        if (!strcmp(err.what(),"Allocator::alloc - Allocator was not initialized")) {
            std::cout << "Uninitialized alloc test - correct" << std::endl;
            return 0;
        }
        std::cout << "Uninitialized alloc test - failed" << std::endl;
        return 1;
    }
    return 1;
}

int
uninitialized_reset()
{
    try
    {
        Allocator x;
        x.reset();
    }
    catch (std::logic_error err) 
    {
        if (!strcmp(err.what(), "Allocator::reset - Allocator was not initialized")) {
            std::cout << "Uninitialized reset test - correct" << std::endl;
            return 0;
        }
        std::cout << "Uninitialized reset test - failed" << std::endl;
        return 1;
    }
    return 1;
}

int
reinitialization()
{
    try
    {
        Allocator x;
        x.makeAllocator(SIZE);
        x.makeAllocator(SIZE);
    }
    catch (std::logic_error err) 
    {
        if (!strcmp(err.what(), "Allocator::makeAllocator - Allocator has already been initialized")) {
            std::cout << "Reinitialization test - correct" << std::endl;
            return 0;
        }
        std::cout << "Reinitialization test - failed" << std::endl;
        return 1;
    }
    return 1;
}

int
main() 
{
    int res = 0;
    res += big_alloc();
    res += many_allocs();
    res += reset_alloc();
    res += uninitialized_alloc();
    res += uninitialized_reset();
    res += reinitialization();
    if (res == 0) {
        std::cout << std::endl << "All tests passed" << std::endl;
    } else if (res == 1){
        std::cout << std::endl << "Total: " << res << " test failed" << std::endl;
    } else{
        std::cout << std::endl << "Total: " << res << " tests failed" << std::endl;
    } 
    return 0;
}