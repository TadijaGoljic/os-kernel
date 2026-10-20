#ifndef _allocator
#define _allocator

#include "../lib/hw.h"

class Allocator {
public:

    static void *allocate(size_t size);

    static int deallocate(void *mem);

    static size_t getFreeSpace();

    static size_t getLargestFree();

private:

    struct Elem {
        Elem *next;
        size_t size;
        void *space;
    };

    static Elem *first;

};

#endif