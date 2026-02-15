#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

static const size_t ARR_INIT_CAP = 8;

/**
 * @invariant:
 *      a != NULL
 *      a->element_size > 0
 *      a->size <= a->capacity
 *      a->capacity > 0
 *      a->data != NULL
 *      a->capacity <= SIZE_MAX / a->element_size
 */
struct Array
{
    void *data;
    size_t size;
    size_t capacity;
    size_t element_size;
};

static inline int array_invariant(const Array *a)
{
    if(!a)
    {
        return 0;
    }

    if(a->element_size == 0)
    {
        return 0;
    }

    if(a->size > a->capacity)
    {
        return 0;
    }

    if(a->capacity == 0 && a->data != NULL)
    {
        return 0;
    }

    if(a->capacity == 0 && a->data == NULL)
    {
        return 0;
    }

    if(a->element_size != 0 && a->capacity > SIZE_MAX / a->element_size)
    {
        return 0;
    }

    return 1;
}

/**
 * @brief Computes a + b with overflow detection for size_t.
 *
 * Domain: a, b -> [0, SIZE_MAX]
 *
 * @pre out != NULL
 *
 * @post On success (return == 0):
 *          - a + b <= SIZE_MAX
 *          - *out == a + b
 *
 * @post On overflow (return == EOVERFLOW):
 *          - a + b > SIZE_MAX
 *          - *out is unchanged
 *
 * @post On invalid argument (return == EINVAL):
 *          - out == NULL
 *
 * @return 0 on success
 *         EINVAL    if out == NULL
 *         EOVERFLOW if addition would overflow
 *
 * @note The function guarantees absence of unsigned wraparound when returning
 * success.
 */
static inline int add_overflow(size_t *out, size_t a, size_t b)
{
    if(out == NULL)
    {
        return EINVAL;
    }

    if(b > SIZE_MAX - a)
    {
        return EOVERFLOW;
    }

    *out = a + b;

    return 0;
}

/**
 * @brief Computes a - b with overflow detection for size_t
 *
 * Domain: a, b -> [0, SIZE_MAX]
 *
 * @pre out != NULL
 *
 * @post On success (return == 0):
 *          - a >= b
 *          - *out == a - b
 *
 * @post On overflow (return == EOVERFLOW)
 *          - a < b
 *          - *out is unchanged
 *
 * @post On invalid argument (return == EINVAL)
 *          - out == NULL
 *
 * @return 0 on success
 *         EINVAL    if out == NULL
 *         EOVERFLOW if subtraction would overflow
 *
 * @note The function guarantees absence of unsigned wraparound when returning
 * success.
 */
static inline int sub_overflow(size_t *out, size_t a, size_t b)
{
    if(out == NULL)
    {
        return EINVAL;
    }

    if(a < b)
    {
        return EOVERFLOW;
    }

    *out = a - b;

    return 0;
}

/**
 * @brief Computes a * b with overflow detection for size_t
 *
 * Domain: a, b -> [0, SIZE_MAX]
 *
 * @pre out != NULL
 *
 * @post On success (return == 0):
 *          - b != 0
 *          - a * b <= SIZE_MAX
 *          - *out = a * b
 *
 * @post On overflow (return == EOVERFLOW):
 *          - a * b > SIZE_MAX
 *          - *out is unchanged
 *
 * @post On invalid argument (return == EINVAL):
 *          - out == NULL
 *
 * @return 0 on success
 *         EINVAL    if out == NULL
 *         EOVERFLOW if subtraction would overflow
 *
 * @note The function guarantees absence of unsigned wraparound when returning
 * success.
 */
static inline int mul_overflow(size_t *out, size_t a, size_t b)
{
    if(out == NULL)
    {
        return EINVAL;
    }

    if(a != 0 && b > SIZE_MAX / a)
    {
        return EOVERFLOW;
    }

    *out = a * b;

    return 0;
}

/**
 * @brief Allocates and initializes a dynamic array.
 *
 * Domain: element_size -> [1, SIZE_MAX]
 *
 * @pre element_size > 0
 *
 * @post On success (return != NULL):
 *          - a->size == 0
 *          - a->capacity == ARR_INIT_CAP
 *          - a->element_size == element_size
 *          - a->data != NULL
 *          - ARR_INIT_CAP * element_size <= SIZE_MAX
 *
 * @post On failure (return == NULL):
 *          - no memory is leaked
 *
 * @note Guarantees that the returned object satisfies all Array invariants.
 */
Array *array_init(size_t element_size)
{
    if(element_size == 0)
    {
        return NULL;
    }

    size_t new_bytes;
    if(mul_overflow(&new_bytes, ARR_INIT_CAP, element_size) != 0)
    {
        return NULL;
    }

    Array *tmp = malloc(sizeof(*tmp));
    if(!tmp)
    {
        return NULL;
    }

    void *data = malloc(new_bytes);
    if(!data)
    {
        free(tmp);
        return NULL;
    }

    tmp->data = data;
    tmp->size = 0;
    tmp->element_size = element_size;
    tmp->capacity = ARR_INIT_CAP;

    return tmp;
}

/**
 * @brief Grows array capacity if needed to fit at least one more element.
 *
 * Increases capacity (usually by factor of 2) using realloc.
 * Safe to call even when capacity is already sufficient.
 *
 * @pre a != NULL
 * @pre a->element_size > 0
 *
 * @post On success:
 *          a->capacity >= a->size + 1
 *          a->data may be reallocated (pointer may change)
 *
 * @post On failure:
 *          array unchanged
 *          capacity may increase
 *
 * @note Not thread-safe
 *
 * @return 0 on success, error code otherwise
 */
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
    if(mul_overflow(&new_bytes, new_capacity, a->element_size) != 0)
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
    if(mul_overflow(&new_bytes, a->size, a->element_size) != 0)
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
 * @brief Delete and free array object, sets pointer to NULL.
 *
 * @pre a != NULL
 *
 * @post *a = NULL
 * @post all variables freed
 *
 * @return void
 */
void array_delete(Array **a)
{
    if(!a || !*a)
    {
        return;
    }

    free((*a)->data);
    free(*a);
    *a = NULL;
}

/**
 * @brief Inserts value at position `index`, shifts rest right.
 *
 * @invariant a->data != NULL if capacity > 0
 * @invariant a->size <= a->capacity
 * @invariant a->element_size > 0
 *
 * @pre a != NULL
 * @pre value != NULL
 * @pre index <= a->size
 *
 * @post On success:
 *          - size increased by 1
 *          - element at index == *value
 *          - elements [index+1, new_size-1] == old[index, old_size-1]
 *          - relative order is preserved
 *
 * @post On failure:
 *          - contents and size unchanged
 *          - capacity may increase
 *
 * @note Not thread-safe.
 * @return 0 on success, error code otherwise.
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
    if(mul_overflow(&insert_offset, index, a->element_size) != 0)
    {
        return EOVERFLOW;
    }

    size_t tail_bytes;
    const size_t tail_count = (a->size - index); // last bytes after index
    if(mul_overflow(&tail_bytes, tail_count, a->element_size) != 0)
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

/**
 * @brief Erases element at position 'index', shifts rest left.
 *
 * @pre a != NULL
 * @pre index <= a->size
 *
 * @post On success:
 *          - size decreased by 1
 *          - element at index == a->size[n+1]
 *          - elements [index-1, new_size+1] == old[index+1, old_size+1]
 *          - relative order preserved
 *
 * @post On failure:
 *          - size and contents unchanged
 *
 * @note Not thread-safe.
 * @return 0 on success, error code otherwise.
 */
int array_erase(Array *a, size_t index)
{
    // entry check
    if(!a || a->element_size == 0 || index >= a->size)
    {
        return EINVAL;
    }

    // calculate bytes after index
    const size_t tail_bytes_count = a->size - (index - 1);

    if(tail_bytes_count > 0)
    {
        size_t bytes_to_move;
        if(mul_overflow(&bytes_to_move, tail_bytes_count, a->element_size) != 0)
        {
            return EOVERFLOW;
        }

        size_t dst_offset;
        if(mul_overflow(&dst_offset, index, a->element_size) != 0)
        {
            return EOVERFLOW;
        }

        size_t src_offset;
        if(mul_overflow(&src_offset, (index + 1), a->element_size) != 0)
        {
            return EOVERFLOW;
        }

        char *base = (char *)a->data;

        void *dst = base + dst_offset;
        void *src = base + src_offset;

        memmove(dst, src, bytes_to_move);

        size_t new_size;
        if(sub_overflow(&new_size, a->size, 1) != 0)
        {
            return EOVERFLOW;
        }
        a->size = new_size;
    }

    return 0;
}

/**
 * @brief Pushes value to the first index, shifts rest right.
 *
 * @pre a != NULL
 * @pre a->element_size > 0
 * @pre value != NULL
 *
 * @post On success:
 *          - value at index == *value
 *          - size increased by 1
 *          - relative order is preserved
 *
 * @post On failure:
 *          - contents and size unchanged
 *          - capacity may increase
 *
 * @note Not thread-safe.
 * @return 0 on success, error code otherwise.
 */
int array_push_front(Array *a, const void *value)
{
    // entry check
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
    if(mul_overflow(&bytes_to_move, a->size, a->element_size) != 0)
    {
        return EOVERFLOW;
    }

    // calculate first index
    size_t dst_offset;
    if(mul_overflow(&dst_offset, a->size, a->element_size) != 0)
    {
        return EOVERFLOW;
    }

    char *base = (char *)a->data;

    void *dst = base + dst_offset;
    void *src = base;

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
    if(!a || !value || a->element_size == 0)
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

    size_t dst_offset;
    if(mul_overflow(&dst_offset, a->size, a->element_size) != 0)
    {
        return EOVERFLOW;
    }

    char *base = (char *)a->data;

    void *dst = base + dst_offset;

    memcpy(dst, value, a->element_size);

    size_t new_size;
    if(add_overflow(&new_size, a->size, 1) != 0)
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

    size_t new_size;
    if(sub_overflow(&new_size, a->size, 1) != 0)
    {
        return;
    }
    a->size = new_size;
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

    size_t new_size;
    if(sub_overflow(&new_size, a->size, 1) != 0)
    {
        return;
    }
    a->size = new_size;

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
