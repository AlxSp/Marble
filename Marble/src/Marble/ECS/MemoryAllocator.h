#pragma once
#include "stdlib.h"

class MemoryAllocator
{
    public:
        virtual void* allocate(size_t size) = 0;
        virtual void  free(void* p) = 0;
};