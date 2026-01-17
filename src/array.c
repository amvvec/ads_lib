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

/**
 * Calculates multiplication overflow
 *
 * @pre out != NULL
 *
 * @post On success:
 *       - *out = a * b
 *
 * @post On failure:
 *       - *out is not changed
 *
 * @return 0 on success, error code otherwise
 */
static inline int multiply_overflow(size_t *out, size_t a, size_t b)
{
    if(!out)
    {
        return EINVAL;
    }

    if(b == 0)
    {
        *out = 0;
        return 0;
    }

    if(a > (SIZE_MAX / b))
    {
        return EOVERFLOW;
    }

    *out = (a * b);

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
    if(!a || a->element_size == 0)
    {
        return EINVAL;
    }

    if(a->capacity >= (a->size + 1))
    {
        return 0; // enough memory
    }

    if(a->capacity > (SIZE_MAX / 2))
    {
        return EOVERFLOW;
    }

    size_t new_capacity = a->capacity ? a->capacity * 2 : ARR_INIT_CAP;

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

/**
 * Delete and free array object
 *
 * @pre a != NULL
 *
 * @post On success:
 *       - *a = NULL
 *       - all object variables are zeroed
 *
 * @post On failure:
 *       - *a is not changed
 *
 * @return zeroed array, NULL otherwise
 */
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
 * Inserts element at position `index` and shifts subsequent elements right.
 *
 * Preconditions:
 *   • a            != NULL
 *   • value        != NULL
 *   • index        <= array_size(a)
 *   • element_size >  0
 *
 * On success:
 *   • array_size(a) increased by 1
 *   • new element at index == *value
 *   • elements [index+1, old_size] == old elements [index, old_size-1]
 *   • relative order of other elements preserved
 *
 * On failure:
 *   • array contents and size unchanged
 *   • capacity may be increased
 *
 * @note Not thread-safe. Caller must ensure exclusive access.
 */
int array_insert(Array *a, const void *restrict value, size_t index)
{
    if(!a || !value || index > a->size || a->element_size == 0)
    {
        return EINVAL;
    }

    int error = array_capacity_grow(a);
    if(error != 0)
    {
        return error;
    }

    size_t insert_offset; // first bytes before index
    if(multiply_overflow(&insert_offset, index, a->element_size) != 0)
    {
        return EOVERFLOW;
    }

    size_t tail_bytes;
    const size_t tail_count = (a->size - index); // last bytes after index
    if(multiply_overflow(&tail_bytes, tail_count, a->element_size) != 0)
    {
        return EOVERFLOW;
    }

    char *base = (char *)a->data;

    // early push_back return
    if(tail_count == 0)
    {
        memcpy(base + insert_offset, value, a->element_size);

        size_t new_size;
        if(add_overflow(&new_size, a->size, 1))
        {
            return EOVERFLOW;
        }
        a->size = new_size;

        return 0;
    }

    void *dst = base + insert_offset + a->element_size;
    void *src = base + insert_offset;

    // safe overlapping move
    memmove(dst, src, tail_bytes);

    // insert new value into freed slot
    memcpy(src, value, a->element_size);

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
    if(!a || a->element_size == 0 || index >= a->size)
    {
        return EINVAL;
    }

    const size_t tail_count = (a->size - index - 1);

    if(tail_count > 0)
    {
        size_t bytes_to_move;
        if(multiply_overflow(&bytes_to_move, tail_count, a->element_size) != 0)
        {
            return EOVERFLOW;
        }

        char *base = (char *)a->data;

        void *dst = base + index * a->element_size;
        void *src = base + (index + 1) * a->element_size;

        memmove(dst, src, bytes_to_move);

        // TODO: add overflow check
        a->size--;
    }

    return 0;
}

int array_push_front(Array *a, const void *value)
{
    // entry validation
    if(!a || !value || a->element_size == 0)
    {
        return EINVAL;
    }

    // capacity ensurance
    if(a->capacity == 0)
    {
        int error = array_capacity_grow(a);
        if(error != 0)
        {
            return error;
        }
    }

    // invariant check after capacity grow
    if(!a->data || a->capacity < a->size)
    {
        return EINVAL;
    }

    size_t bytes_to_move;
    if(multiply_overflow(&bytes_to_move, a->size, a->element_size) != 0)
    {
        return EOVERFLOW;
    }

    // calculate first index
    void *dst = (char *)a->data + 1 * a->element_size; // not safe
    void *src = (char *)a->data;

    memmove(dst, src, bytes_to_move);
    memcpy(a->data, value, a->element_size);

    size_t new_size;
    if(add_overflow(&new_size, a->size, 1) != 0)
    {
        return EOVERFLOW;
    }
    a->size = new_size;

    return 0;
}

int array_push_back(Array *a, const void *value)
{
    if(!a || !value)
    {
        return EINVAL;
    }

    if(a->capacity <= a->size)
    {
        int error = array_capacity_grow(a);
        if(error)
        {
            return error;
        }
    }

    // absolutly not safe
    void *dst = (char *)a->data + (a->size * a->element_size);

    memcpy(dst, value, a->element_size);

    size_t new_size;
    if(multiply_overflow(&new_size, a->size, 1) != 0)
    {
        return EOVERFLOW;
    }
    a->size = new_size;

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
    if(!a)
    {
        return EINVAL;
    }

    return a ? a->size : 0;
}

size_t array_capacity(const Array *a)
{
    if(!a)
    {
        return EINVAL;
    }

    return a ? a->capacity : 0;
}
