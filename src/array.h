#ifndef ARRAY_H
#define ARRAY_H

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
int node_array_init(Array* array);
Array* node_array_new(void);

// Memory management
// static int node_array_grow_to(Array* array, size_t new_capacity);

void node_array_free(Array* a);
void node_array_delete();

#endif // !ARRAY_H
