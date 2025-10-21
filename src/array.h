#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <stdint.h>

struct ArrayNode
{
    int data;
};

typedef struct
{
    struct ArrayNode* data;
    size_t size;
    size_t capacity;
} Array;

// Initialization
int array_init(Array* a);
Array* array_new(void);

// Memory management
int array_grow_to(Array* a, size_t new_capacity);

void array_free(Array* a);
void array_delete(Array* a);

#endif // !ARRAY_H
