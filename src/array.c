#include <errno.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

enum
{
    ARRAY_INIT_CAP = 8u
};

static const size_t MAX_ELEMENT_SIZE = (SIZE_MAX / ARRAY_INIT_CAP);

typedef struct Array
{
    void *data;
    size_t size;
    size_t capacity;
    size_t element_size;
} Array;

static int is_valid_element_size(size_t size)
{
    return (size > 0u) && (size <= MAX_ELEMENT_SIZE);
}

Array *array_init(size_t element_size)
{
    if(!is_valid_element_size(element_size))
    {
        return NULL;
    }

    Array *a = malloc(sizeof(*a));
    if(a == NULL)
    {
        return NULL;
    }

    a->data = NULL;
    a->size = 0u;
    a->capacity = 0u;
    a->element_size = element_size;

    return a;
}

static int mult_overflow_size_t(size_t *out_bytes, size_t element_count,
                                size_t element_size)
{
    if(out_bytes == NULL)
    {
        return EINVAL;
    }

    if((element_size == 0u) || (element_size > MAX_ELEMENT_SIZE))
    {
        return EINVAL;
    }

    if(element_count > (SIZE_MAX / element_size))
    {
        return EOVERFLOW;
    }

    *out_bytes = (element_count * element_size);

    return 0;
}

static int array_grow_to(Array *a, size_t start_capacity)
{
    if(a == NULL)
    {
        return EINVAL;
    }
    if(start_capacity <= a->capacity)
    {
        return 0; // enough memory
    }

    size_t new_capacity = a->capacity ? a->capacity : ARRAY_INIT_CAP;
    while(new_capacity < start_capacity)
    {
        if(new_capacity > (MAX_ELEMENT_SIZE / 2u))
        {
            return EOVERFLOW;
        }
        new_capacity *= 2u;
    }

    size_t new_bytes = 0u;
    if(mult_overflow_size_t(&new_bytes, start_capacity, a->element_size))
    {
        return EOVERFLOW;
    }

    void *new_data = realloc(a->data, new_bytes);
    if(new_data == NULL)
    {
        return ENOMEM;
    }

    a->data = new_data;
    a->capacity = new_capacity;

    return 0;
}

int array_shrink_to_fit(Array *a)
{
    if(a == NULL)
    {
        return EINVAL;
    }

    if(a->size == a->capacity)
    {
        return 0; // enough memory
    }

    if(a->size == 0)
    {
        free(a->data);

        a->data = NULL;
        a->capacity = 0;

        return 0;
    }

    size_t new_bytes = 0u;
    if(mult_overflow_size_t(&new_bytes, a->size, a->element_size) != 0)
    {
        return EOVERFLOW;
    }

    void *tmp = realloc(a->data, new_bytes);
    if(tmp == NULL)
    {
        return ENOMEM; // TODO: ?
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

    a->data = NULL;
    a->size = 0u;
    a->capacity = 0u;
    a->element_size = 0u;

    free(a);
}

int array_insert(Array *a, const void *value, size_t index)
{
    if(a == NULL)
    {
        return EINVAL;
    }

    if(value == NULL)
    {
        return EINVAL;
    }

    if(index > a->size)
    {
        return EINVAL;
    }

    if(a->size >= a->capacity)
    {
        size_t new_capacity =
            (a->capacity != 0u) ? (a->capacity * 2u) : ARRAY_INIT_CAP;

        int err = array_grow_to(a, new_capacity);

        if(err != 0)
        {
            return err;
        }
    }

    char *base = (char *)a->data;
    if(index <= a->size)
    {
        size_t src_offset = 0u;
        if(mult_overflow_size_t(&src_offset, index, a->element_size) != 0)
        {
            return EINVAL;
        }

        size_t dst_offset = 0u;
        if(mult_overflow_size_t(&dst_offset, index, a->element_size) != 0)
        {
            return EINVAL;
        }

        size_t bytes_to_move = 0u;
        size_t tail_count = a->size - index;
        if(mult_overflow_size_t(&bytes_to_move, tail_count, a->element_size) !=
           0)
        {
            return EINVAL;
        }

        memmove(base + dst_offset, base + src_offset, bytes_to_move);
    }

    size_t insert_offset = 0u;
    if(mult_overflow_size_t(&insert_offset, index, a->element_size) != 0)
    {
        return EINVAL;
    }

    memcpy(base + insert_offset, value, a->element_size);
    a->size++;

    return 0;
}

int array_erase(Array *a, size_t index)
{
    if(a == NULL)
    {
        return EINVAL;
    }

    if(index >= a->size)
    {
        return EINVAL;
    }

    if(index < (a->size - 1u))
    {
        char *base = (char *)a->data;

        size_t tail_count = a->size - index - 1u;

        size_t src_offset = 0u;
        if(mult_overflow_size_t(&src_offset, index, a->element_size) != 0)
        {
            return EOVERFLOW;
        }

        size_t dst_offset = 0u;
        if(mult_overflow_size_t(&dst_offset, index, a->element_size) != 0)
        {
            return EOVERFLOW;
        }

        size_t bytes_to_move = 0u;
        if(mult_overflow_size_t(&bytes_to_move, tail_count, a->element_size) !=
           0)
        {
            return EOVERFLOW;
        }

        {
            void *dest = base + dst_offset;
            void *src = base + src_offset;

            memmove(dest, src, bytes_to_move);
        }
    }

    a->size--;

    {
        size_t last_offset = 0u;
        if(mult_overflow_size_t(&last_offset, a->size, a->element_size) != 0)
        {
            return EOVERFLOW;
        }

        {
            char *base = (char *)a->data;
            void *last = &base[last_offset];

            memset(last, 0, a->element_size);
        }
    }

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
    // WARNING: something is wrong here
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
