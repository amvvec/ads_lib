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
