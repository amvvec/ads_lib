#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

enum
{
    NODE_ARRAY_INIT_CAP = 8
};

int array_init(Array* a)
{
    if(!a)
    {
        return EINVAL;
    }
    a->data = NULL;
    a->size = 0;
    a->capacity = 0;
    return 0;
}

Array* array_new(void)
{
    Array* a = malloc(sizeof(Array));
    if(!a)
    {
        return NULL;
    }
    array_init(a);
    return a;
}

// static int array_grow_to(Array* a, size_t start_capacity)
// {
//     if(!a)
//     {
//         return EINVAL;
//     }
//     if(start_capacity <= a->capacity)
//     {
//         return 0; // Enough memory
//     }
//     size_t new_capacity = a->capacity ? a->capacity : NODE_ARRAY_INIT_CAP;
//     while(new_capacity < start_capacity)
//     {
//         if(new_capacity > SIZE_MAX / 2)
//         {
//             new_capacity = start_capacity; // No overflow fallback
//             break;
//         }
//         new_capacity *= 2;
//     }
//     size_t new_bytes;
//     struct Node* new_data = realloc(a->data, &new_bytes);
//     if(!new_data)
//     {
//         return ENOMEM; // TODO: EOVERFLOW
//     }
//     a->data = new_data;
//     a->capacity = start_capacity;
//     return 0;
// }

void array_free(Array* a)
{
    if(!a)
    {
        printf("Error: NULL pointer\n");
        return;
    }
    free(a->data);
    a->data = NULL;
    a->size = 0;
    a->capacity = 0;
}

void array_delete(Array* a)
{
    if(!a)
    {
        printf("Error: NULL pointer\n");
        return;
    }
    array_free(a);
    free(a);
}
