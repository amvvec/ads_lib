#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

typedef struct ArrayNode
{
    int data;
} ArrayNode;

typedef struct Array
{
    ArrayNode * data;
    size_t size;
    size_t capacity;
    size_t element_size;
} Array;

enum
{
    ARRAY_INIT_CAP = 8
};

int array_init(Array * a, size_t element_size)
{
    if(!a && element_size == 0)
    {
        return EINVAL;
    }
    a->data = NULL;
    a->size = 0;
    a->capacity = 0;
    return 0;
}

Array * array_new(size_t element_size)
{
    Array * a = malloc(sizeof(Array));
    if(!a)
    {
        return NULL;
    }
    if(array_init(a, element_size) != 0)
    {
        free(a);
        return NULL;
    }
    return a;
}

static int multiply_overflow_size_t(size_t count, size_t element_size,
                                    size_t * out_bytes)
{
    if(out_bytes == NULL || element_size == 0)
    {
        return EINVAL;
    }
    if(count > SIZE_MAX / element_size)
    {
        return EOVERFLOW;
    }
    *out_bytes = count * element_size;
    return 0;
}

int array_grow_to(Array * a, size_t start_capacity)
{
    if(!a)
    {
        return EINVAL;
    }
    if(start_capacity <= a->capacity)
    {
        return 0; // Enough memory
    }
    size_t new_capacity = a->capacity ? a->capacity : ARRAY_INIT_CAP;
    while(new_capacity < start_capacity)
    {
        if(new_capacity > SIZE_MAX / 2)
        {
            return EOVERFLOW; // No overflow fallback
        }
        new_capacity *= 2;
    }
    size_t new_bytes;
    if(multiply_overflow_size_t(start_capacity, sizeof(struct ArrayNode),
                                &new_bytes))
    {
        return EOVERFLOW;
    }
    struct ArrayNode * new_data = realloc(a->data, new_bytes);
    if(!new_data)
    {
        return ENOMEM;
    }
    a->data = new_data;
    a->capacity = new_capacity;
    return 0;
}

void array_free(Array * a)
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

void array_delete(Array * a)
{
    if(!a)
    {
        printf("Error: NULL pointer\n");
        return;
    }
    array_free(a);
    free(a);
}
