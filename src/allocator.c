#include "allocator.h"

struct Allocator
{
    allocator_function allocate;
    reallocator_function reallocate;
    free_function deallocate;
};

#include <stdlib.h>

static void *
system_allocator(size_t size)
{
    return malloc(size);
}

static void *
system_reallocator(void *pointer, size_t size)
{
    return realloc(pointer, size);
}

static void
system_free(void *pointer)
{
    free(pointer);
}

const Allocator global_allocator = {.allocate = system_allocator,
                                    .reallocate = system_reallocator,
                                    .deallocate = system_free};
