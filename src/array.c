#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

enum
{
    NODE_ARRAY_INIT_CAP = 8
};

int node_array_init(Array* array)
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

Array* node_array_new(void)
{
    Array* array = malloc(sizeof(Array));
    if(!array)
    {
        return NULL;
    }
    node_array_init(array);
    return array;
}

static int node_array_grow_to(Array* array, size_t start_capacity)
{
    if(!array)
    {
        return EINVAL;
    }
    if(start_capacity <= array->capacity)
    {
        return 0; // Enough memory
    }
    size_t new_capacity =
        array->capacity ? array->capacity : NODE_ARRAY_INIT_CAP;
    while(new_capacity < start_capacity)
    {
        if(new_capacity > SIZE_MAX / 2)
        {
            new_capacity = start_capacity; // No overflow fallback
            break;
        }
        new_capacity *= 2;
    }
    size_t new_bytes;
    struct Node* new_data = realloc(array->data, new_bytes);
    if(!new_data)
    {
        return ENOMEM; // TODO: EOVERFLOW
    }
    array->data = new_data;
    array->capacity = start_capacity;
    return 0;
}

void node_array_free(Array* array)
{
    if(!array)
    {
        printf("Error: NULL pointer\n");
        return;
    }
    free(array->data);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
}
