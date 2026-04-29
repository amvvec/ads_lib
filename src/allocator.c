#include "allocator.h"

#include <stdlib.h>

void * memory_allocator(size_t size)
{
    return malloc(size);
}

void * memory_reallocator(void * pointer, size_t size)
{
    return realloc(pointer,size);
}

void memory_free(void * pointer)
{
    free(pointer);
}
