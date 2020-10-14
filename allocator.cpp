#include "allocator.h"
#include <exception>
#include <stdexcept>
#include <cstddef>
#include <iostream>

void Allocator::makeAllocator(size_t maxSize) {
    if (size != -1) {
        throw std::logic_error("Allocator::makeAllocator - Allocator has already been initialized");
    }
    mem = new char[maxSize];
    offset = 0;
    size = maxSize;
}

Allocator::Allocator() : mem(nullptr), size(-1) {}

Allocator::~Allocator() {
    if(size != -1) {
        delete[] mem;
    }
}

void Allocator::reset() {
    if (size == -1) {
        throw std::logic_error("Allocator::reset - Allocator was not initialized");
    }
    offset = 0;
    memset(mem, 0, size);
}

char* Allocator::alloc(size_t size) {
    if (this->size == -1) {
        throw std::logic_error("Allocator::alloc - Allocator was not initialized");
    } else if (offset + size > this->size) {
        return nullptr;
    } else {
        offset += size;
        return &mem[offset - size];
    }
}