#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

typedef struct Allocator Allocator;

typedef void *(*allocator_function)(size_t size);
typedef void *(*reallocator_function)(void *pointer, size_t size);
typedef void (*free_function)(void *pointer);

/*
Global default allocator
*/
extern const Allocator global_allocator;

#endif // ALLOCATOR_H
