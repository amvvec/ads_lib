#include <errno.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

static const size_t ARR_INIT_CAP = 8;

/**
 * Array invariants:
 *  - a != NULL
 *  - a->element_size > 0
 *  - a->capacity >= a->size
 *  - a->data != NULL iff a->capacity > 0
 *  - size * element_size does not overflow size_t
 */
typedef struct Array
{
    void *data;
    size_t size;
    size_t capacity;
    size_t element_size;
} Array;

/**
 * Calculates addition overflow
 *
 * @pre out != NULL
 *
 * @post On success:
 *       - *out == a + b
 *
 * @post On failure:
 *       - *out is not changed
 *
 * @return 0 on success, error code otherwise
 */
static inline int add_overflow(size_t *out, size_t a, size_t b)
{
    if(!out)
    {
        return EINVAL;
    }

    if(a > (SIZE_MAX - b))
    {
        return EOVERFLOW;
    }

    *out = a + b;

    return 0;
}

static int multiply_overflow(size_t *out_bytes, size_t count, size_t size)
{
    if(!out_bytes)
    {
        return EINVAL;
    }
    if(size == 0)
    {
        *out_bytes = 0;
        return 0;
    }
    if(count > (SIZE_MAX / size))
    {
        return EOVERFLOW;
    }
    *out_bytes = (count * size);
    return 0;
}

Array *array_init(size_t element_size)
{
    if(element_size == 0)
    {
        return NULL;
    }

    Array *a = malloc(sizeof(*a));
    if(!a)
    {
        return NULL;
    }

    size_t new_bytes;
    if(multiply_overflow(&new_bytes, ARR_INIT_CAP, element_size) != 0)
    {
        free(a);
        return NULL;
    }
    a->data = malloc(new_bytes);

    a->size = 0;
    a->capacity = ARR_INIT_CAP;
    a->element_size = element_size;

    return a;
}

static int array_capacity_grow(Array *a)
{
    if(!a)
    {
        return EINVAL;
    }

    if(a->capacity > a->size)
    {
        return 0; // enough memory
    }

    size_t new_capacity;
    if(a->capacity == 0)
    {
        new_capacity = ARR_INIT_CAP;
    }
    else if(a->capacity > (SIZE_MAX / 2))
    {
        return EOVERFLOW;
    }
    else
    {
        new_capacity = a->capacity * 2;
    }

    if(new_capacity < (a->size + 1))
    {
        return EOVERFLOW;
    }

    size_t new_bytes;
    if(multiply_overflow(&new_bytes, new_capacity, a->element_size) != 0)
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

int array_shrink_fit(Array *a)
{
    if(!a)
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

    size_t new_bytes = 0;
    if(multiply_overflow(&new_bytes, a->size, a->element_size) != 0)
    {
        return EOVERFLOW;
    }

    void *tmp = realloc(a->data, new_bytes);
    if(!tmp)
    {
        return ENOMEM;
    }

    a->data = tmp;
    a->capacity = a->size;

    return 0;
}

void array_delete(Array *a)
{
    if(!a)
    {
        return;
    }

    free(a->data);
    a->data = NULL;

    a->capacity = 0;
    a->element_size = 0;
    a->size = 0;

    free(a);
    a = NULL;
}

/**
 * Inserts an element at given index
 *
 * @pre a != NULL
 * @pre value != NULL
 * @pre index <= array_size(a)
 * @pre a->element_size != 0
 *
 * @post On success:
 *       - a->size is increased by 1
 *       - elements at (index, old_size) are shifted right
 *       - element at index equals *value
 *
 * @post On failure:
 *       - logical contents (size + elements) remain unchanged
 *       - capacity may increase
 *
 * @return 0 on success, error code otherwise
 */
int array_insert(Array *a, const void *value, size_t index)
{
    // entry validation
    if(!a || !value || index > a->size || a->element_size == 0)
    {
        return EINVAL;
    }

    // ensure capacity growth
    int error = array_capacity_grow(a);
    if(error != 0)
    {
        return error;
    }

    size_t insert_offset;
    if(multiply_overflow(&insert_offset, index, a->element_size) != 0)
    {
        return EOVERFLOW;
    }

    size_t tail_bytes;
    const size_t tail_count = (a->size - index);
    if(multiply_overflow(&tail_bytes, tail_count, a->element_size) != 0)
    {
        return EOVERFLOW;
    }

    char *base = (char *)a->data;

    memmove(base + insert_offset + a->element_size, base + insert_offset,
            tail_bytes);

    memcpy(base + insert_offset, value, a->element_size);

    size_t new_size;
    if(add_overflow(&new_size, a->size, 1))
    {
        return EOVERFLOW;
    }
    a->size = new_size;

    return 0;
}

int array_erase(Array *a, size_t index)
{
    if(a == NULL || index >= a->size)
    {
        return EINVAL;
    }

    size_t tail_count = (a->size - index - 1);

    if(tail_count > 0)
    {
        size_t bytes_to_move;
        if(multiply_overflow(&bytes_to_move, tail_count, a->element_size) != 0)
        {
            return EOVERFLOW;
        }

        char *base = (char *)a->data;

        memmove(base + index * a->element_size,
                base + (index + 1) * a->element_size, bytes_to_move);

        a->size--;
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
        int error = array_capacity_grow(a);
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
        int error = array_capacity_grow(a);
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
