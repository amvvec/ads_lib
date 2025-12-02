#include <errno.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

typedef struct Array
{
    void *data;
    size_t size;
    size_t capacity;
    size_t element_size;
} Array;

enum
{
    ARRAY_INIT_CAP = 8
};

Array *array_init(size_t element_size)
{
    if(element_size == 0 || element_size > SIZE_MAX / 8)
    {
        return NULL;
    }
    Array *a = malloc(sizeof *a);
    if(!a)
    {
        return NULL;
    }
    a->data = NULL;
    a->capacity = 0;
    a->size = 0;
    a->element_size = element_size;
    return a;
}

static int mult_overflow_size_t(size_t count, size_t element_size,
                                size_t *out_bytes)
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

static int array_grow_to(Array *a, size_t start_capacity)
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
            return EOVERFLOW;
        }
        new_capacity *= 2;
    }
    size_t new_bytes;
    if(mult_overflow_size_t(start_capacity, a->element_size, &new_bytes))
    {
        return EOVERFLOW;
    }
    void *new_data = realloc(a->data, new_bytes);
    if(!new_data)
    {
        return ENOMEM;
    }
    a->data = new_data;
    a->capacity = new_capacity;
    return 0;
}

int array_shrink_to_fit(Array *a)
{
    if(!a)
    {
        return EINVAL;
    }
    if(a->size == a->capacity)
    {
        return 0;
    }
    if(a->size == 0)
    {
        free(a->data);
        a->data = NULL;
        a->capacity = 0;
        return 0;
    }
    size_t new_bytes;
    if(mult_overflow_size_t(a->size, a->element_size, &new_bytes) != 0)
    {
        return EOVERFLOW;
    }
    void *tmp = realloc(a->data, new_bytes);
    if(!tmp)
    {
        return ENOMEM; // TODO
    }
    a->data = tmp;
    a->capacity = a->size;
    return 0;
}

void array_delete(Array *a)
{
    if(a == NULL)
    {
        return;
    }
    free(a->data);
    free(a);
}

int array_insert(Array *a, size_t index, const void *value)
{
    if(!a || !value)
    {
        return EINVAL;
    }
    if(index > a->size)
    {
        return EINVAL;
    }
    if(a->size >= a->capacity)
    {
        int error =
            array_grow_to(a, a->capacity ? a->capacity * 2 : ARRAY_INIT_CAP);
        if(error)
        {
            return error;
        }
    }
    char *base = (char *)a->data;
    if(index < a->size)
    {
        void *dest = base + (index + 1) * a->element_size;
        void *src = base + index * a->element_size;
        size_t bytes_to_move = (a->size - index) * a->element_size;
        memmove(dest, src, bytes_to_move);
    }
    void *insert = base + index * a->element_size;
    memcpy(insert, value, a->element_size);
    a->size++;
    return 0;
}

int array_erase(Array *a, size_t index)
{
    if(!a)
    {
        return EINVAL;
    }
    if(index >= a->size)
    {
        return EINVAL;
    }
    if(index < a->size - 1)
    {
        char *base = (char *)a->data;
        void *dest = base + index * a->element_size;
        void *src = base + (index + 1) * a->element_size;
        size_t bytes_to_move = (a->size - index - 1) * a->element_size;
        memmove(dest, src, bytes_to_move);
    }
    a->size--;
    void *last = (char *)a->data + a->size * a->element_size;
    memset(last, 0, a->element_size);
    return 0;
}

int array_push_front(Array *a, const void *value)
{
    if(!a || !value)
    {
        return EINVAL;
    }
    if(a->size >= a->capacity)
    {
        int error =
            array_grow_to(a, a->capacity ? a->capacity * 2 : ARRAY_INIT_CAP);
        if(error)
        {
            return error;
        }
    }
    void *dest = (char *)a->data + 1 * a->element_size;
    void *src = (char *)a->data;
    size_t bytes_to_move = a->size * a->element_size;
    memmove(dest, src, bytes_to_move);
    memcpy(a->data, value, a->element_size);
    a->size++;
    return 0;
}

int array_push_back(Array *a, const void *value)
{
    if(!a && !value)
    {
        return EINVAL;
    }
    if(a->size >= a->capacity)
    {
        int error =
            array_grow_to(a, a->capacity ? a->capacity * 2 : ARRAY_INIT_CAP);
        if(error)
        {
            return error;
        }
    }
    void *dest = (char *)a->data + a->size * a->element_size;
    memcpy(dest, value, a->element_size);
    a->size++;
    return 0;
}

void array_pop_front(Array *a)
{
    if(!a || a->size == 0)
    {
        return;
    }
    // WARNING: smth is wrong here
    // size_t bytes_to_move = (size_t)(a->size - 1) * (size_t)a->capacity;
    size_t bytes_to_move = (a->size - 1) * a->capacity;
    memmove(a->data, (char *)a->data + a->element_size, bytes_to_move);
    a->size--;
}

void array_pop_back(Array *a)
{
    if(!a)
    {
        return;
    }
    if(a->size == 0)
    {
        return;
    }
    a->size--;
    void *dest = (char *)a->data + a->size * a->element_size;
    memset(dest, 0, a->element_size);
}

int array_get(const Array *a, size_t index, void *out_value)
{
    if(!a || !out_value || index >= a->size)
    {
        return EINVAL;
    }
    const char *base = a->data;
    const void *src = base + index * a->element_size;
    memcpy(out_value, src, a->element_size);
    return 0;
}

int array_set(Array *a, size_t index, const void *value)
{
    if(!a || !value || index >= a->size)
    {
        return EINVAL;
    }
    char *base = a->data;
    void *dest = base + index * a->element_size;
    memcpy(dest, value, a->element_size);
    return 0;
}

size_t array_size(const Array *a)
{
    return a ? a->size : 0;
}

size_t array_capacity(const Array *a)
{
    return a ? a->capacity : 0;
}
