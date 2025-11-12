#include <errno.h>
#include <memory.h>
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

int array_init(Array * array, size_t element_size)
{
    if(!array && element_size == 0)
    {
        return EINVAL;
    }
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
    array->element_size = element_size;
    return 0;
}

Array * array_new(size_t element_size)
{
    if(element_size == 0)
    {
        return NULL;
    }
    Array * array = malloc(sizeof(Array));
    if(!array)
    {
        return NULL;
    }
    if(array_init(array, element_size) != 0)
    {
        free(array);
        return NULL;
    }
    return array;
}

static int multiply_overflow_size_t(size_t count, size_t element_size,
                                    size_t * out_bytes)
{
    if(!out_bytes || element_size == 0)
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

static int array_grow_to(Array * array, size_t start_capacity)
{
    if(!array)
    {
        return EINVAL;
    }
    if(start_capacity <= array->capacity)
    {
        return 0; // Enough memory
    }
    size_t new_capacity = array->capacity ? array->capacity : ARRAY_INIT_CAP;
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
    struct ArrayNode * new_data = realloc(array->data, new_bytes);
    if(!new_data)
    {
        return ENOMEM;
    }
    array->data = new_data;
    array->capacity = new_capacity;
    return 0;
}

void array_free(Array * array)
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

void array_delete(Array * array)
{
    if(!array)
    {
        printf("Error: NULL pointer\n");
        return;
    }
    array_free(array);
    free(array);
}

int array_insert(Array * array, size_t index, const void * value)
{
    if(!array || !value)
    {
        return EINVAL;
    }
    if(index > array->size)
    {
        return EINVAL;
    }
    if(array->size >= array->capacity)
    {
        int error = array_grow_to(array, array->capacity ? array->capacity * 2
                                                         : ARRAY_INIT_CAP);
        if(error)
        {
            return error;
        }
    }
    char * base = (char *)array->data;
    if(index < array->size)
    {
        void * dest = base + (index + 1) * array->element_size;
        void * src = base + index * array->element_size;
        size_t bytes_to_move = (array->size - index) * array->element_size;
        memmove(dest, src, bytes_to_move);
    }
    void * insert = base + index * array->element_size;
    memcpy(insert, value, array->element_size);
    array->size++;
    return 0;
}

int array_erase(Array * array, size_t index)
{
    if(!array)
    {
        return EINVAL;
    }
    if(index >= array->size)
    {
        return EINVAL;
    }
    if(index < array->size - 1)
    {
        char * base = (char *)array->data;
        void * dest = base + index * array->element_size;
        void * src = base + (index + 1) * array->element_size;
        size_t bytes_to_move = (array->size - index - 1) * array->element_size;
        memmove(dest, src, bytes_to_move);
    }
    array->size--;
    void * last = (char *)array->data + array->size * array->element_size;
    memset(last, 0, array->element_size);
    return 0;
}

int array_push_front(Array * array, const void * value)
{
    if(!array || !value)
    {
        return EINVAL;
    }
    if(array->size >= array->capacity)
    {
        int error = array_grow_to(array, array->capacity ? array->capacity * 2
                                                         : ARRAY_INIT_CAP);
        if(error)
        {
            return error;
        }
    }
    void * dest = (char *)array->data + 1 * array->element_size;
    void * src = (char *)array->data;
    size_t bytes_to_move = array->size * array->element_size;
    memmove(dest, src, bytes_to_move);
    memcpy(array->data, value, array->element_size);
    array->size++;
    return 0;
}

int array_push_back(Array * array, const void * value)
{
    if(!array && !value)
    {
        return EINVAL;
    }
    if(array->size >= array->capacity)
    {
        int error = array_grow_to(array, array->capacity ? array->capacity * 2
                                                         : ARRAY_INIT_CAP);
        if(error)
        {
            return error;
        }
    }
    void * dest = (char *)array->data + array->size * array->element_size;
    memcpy(dest, value, array->element_size);
    array->size++;
    return 0;
}

void array_pop_front(Array * array)
{
    if(!array || array->size == 0)
    {
        return;
    }
    size_t bytes_to_move =
        (array->size - 1) * array->capacity; // WARNING: smth is wrong here
    memmove(array->data, (char *)array->data + array->element_size,
            bytes_to_move);
    array->size--;
}

void array_pop_back(Array * array)
{
    if(!array)
    {
        return;
    }
    if(array->size == 0)
    {
        return;
    }
    array->size--;
    void * dest = (char *)array->data + array->size * array->element_size;
    memset(dest, 0, array->element_size);
}

int array_get(const Array * array, size_t index, int * out_value)
{
    if(!array || !out_value || index >= array->size)
    {
        return EINVAL;
    }
    *out_value = array->data[index].data;
    return 0;
}

int array_set(Array * array, size_t index, int value)
{
    if(!array || index >= array->size)
    {
        return EINVAL;
    }
    array->data[index].data = value;
    return 0;
}

size_t array_size(const Array * array)
{
    return array ? array->size : 0;
}

size_t array_capacity(const Array * array)
{
    return array ? array->capacity : 0;
}
