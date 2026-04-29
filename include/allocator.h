#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

void * memory_allocator(size_t size);
void * memory_reallocator(void * pointer, size_t size);
void memory_free(void * pointer);

#endif // ALLOCATOR_H
