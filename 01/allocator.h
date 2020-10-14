#ifndef ALLOCATOR_H
#define ALLOCATOR_H
#include <string.h>

class Allocator
{
public:
    void makeAllocator(size_t maxSize);
    char* alloc(size_t size);
    void reset();
    Allocator();
    ~Allocator();
private:
    char *mem;
    long long offset;
    long long size;
};

#endif