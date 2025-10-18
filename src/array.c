#include <errno.h>
#include <stdlib.h>

#include "array.h"

int node_array_init(NodeArray* array)
{
    if(!array)
    {
        return EINVAL;
    }
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
    return 0;
}

NodeArray* node_array_new(void)
{
    NodeArray* array = malloc(sizeof(NodeArray));
    if(!array)
    {
        return NULL;
    }
    node_array_init(array);
    return array;
}

static int node_array_grow_to(NodeArray* array, size_t new_capacity)
{
    if(!array)
    {
        return EINVAL;
    }
    if(new_capacity <= array->capacity)
    {
        return 0; // enough memory
    }
    size_t new_bytes;
    struct Node* new_data = realloc(array->data, new_bytes);
    if(!new_data)
    {
        return ENOMEM;
    }
    array->data = new_data;
    array->capacity = new_capacity;
    return 0;
}
