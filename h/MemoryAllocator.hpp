//
// Created by os on 8/13/24.
//

#ifndef MEMORYALLOCATOR_HPP
#define MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

struct FreeMem{
    FreeMem* next;
    FreeMem* prev;
    size_t size;
    bool freed;
};

class MemoryAllocator{

public:

    static FreeMem* fmem_head;

    static void* mem_alloc(size_t size);
    static int mem_free(void* adr);

    static void tryToJoin(FreeMem* cur);


};

#endif //MEMORYALLOCATOR_HPP