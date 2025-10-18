#ifndef ARRAY_H
#define ARRAY_H

#include <stdint.h>

struct Node
{
    int data;
};

typedef struct
{
    struct Node* data;
    size_t size;
    size_t capacity;
} NodeArray;

int node_array_init(NodeArray* array);
NodeArray* node_array_new(void);

#endif // !ARRAY_H
