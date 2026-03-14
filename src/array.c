#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/array.h"

static const size_t ARRAY_INITIAL_CAPACITY = 8;

/**
 * @invariant:
 *          - a != NULL
 *          - a->element_size > 0
 *          - a->size <= a->capacity
 *          - a->capacity > 0
 *          - a->data != NULL
 *          - a->capacity <= SIZE_MAX / a->element_size
 */
struct Array
{
    void *data;
    size_t size;
    size_t capacity;
    size_t element_size;
};

static inline int array_invariant_check(const Array *a)
{
    if(!a)
    {
        return EINVAL;
    }
    if(a->element_size == 0)
    {
        return EINVAL;
    }
    if(a->size > a->capacity)
    {
        return EINVAL;
    }

    if(a->capacity == 0)
    {
        if(a->data != NULL)
        {
            return EINVAL;
        }
    }
    else
    {
        if(a->data == NULL)
        {
            return EINVAL;
        }
    }

    if(a->capacity > SIZE_MAX / a->element_size)
    {
        return EOVERFLOW;
    }

    return 0;
}

#ifdef ARRAY_DEBUG
#define ARRAY_ASSERT(a)                                                        \
    do                                                                         \
    {                                                                          \
        if(array_invariant_check(a) != 0)                                      \
        {                                                                      \
            abort();                                                           \
        }                                                                      \
    } while(0)
#else
#define ARRAY_ASSERT(a) ((void)0)
#endif

static inline int
add_overflow(size_t *out, size_t a, size_t b)
{
    if(!out) return EINVAL;
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_add_overflow(a, b, out);
#else
    if(a > SIZE_MAX - b) return EOVERFLOW;
    *out = a + b;
    return 0;
#endif
}

static inline int
sub_overflow(size_t *out, size_t a, size_t b)
{
    if(!out) return EINVAL;
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_sub_overflow(a, b, out);
#else
    if(a < b) return EOVERFLOW;
    *out = a - b;
    return 0;
#endif
}

static inline int
mul_overflow(size_t *out, size_t a, size_t b)
{
    if(!out) return EINVAL;
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_mul_overflow(a, b, out);
#else
    if(b != 0 || a > SIZE_MAX / b) return EOVERFLOW;
    *out = a * b;
    return 0;
#endif
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
 *          - a->capacity == ARRAY_INITIAL_CAPACITY
 *          - a->element_size == element_size
 *          - a->data != NULL
 *          - ARRAY_INITIAL_CAPACITY * element_size <= SIZE_MAX
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

    assert(ARRAY_INITIAL_CAPACITY > 0);

    size_t new_bytes;
    if(mul_overflow(&new_bytes, ARRAY_INITIAL_CAPACITY, element_size) != 0)
    {
        return NULL;
    }

    Array *tmp = calloc(1, sizeof(*tmp));
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
    tmp->capacity = ARRAY_INITIAL_CAPACITY;

    return tmp;
}

/**
 * @brief Destroys the array and releases all owned memory.
 *
 * Domain: a may be NULL, *a may be NULL.
 *
 * @post If a != NULL and *a != NULL:
 *          - the internal buffer is freed
 *          - the Array object is freed
 *          - *a is set to NULL
 *
 * @post If a == NULL or *a == NULL:
 *          - no action is performed
 *
 * @note This function is idempotent when called with the same pointer.
 */
void array_delete(Array **a)
{
    if(a == NULL)
    {
        return;
    }

    if(*a)
    {
        free((*a)->data);
        free(*a);

        *a = NULL;
    }
}

static inline int
array_reserve(Array *a)
{
    ARRAY_ASSERT(a);
    
    if(!a) return EINVAL;

    size_t requiered_size;
    if(add_overflow(&requiered_size, a->size, 1)) return EOVERFLOW;

    if(a->capacity >= requiered_size) return 0; // enough capacity

    if(a->capacity > SIZE_MAX / 2) return EOVERFLOW;

    assert(ARRAY_INITIAL_CAPACITY > 1);
    
    size_t new_capacity = a->capacity ? (a->capacity * 2) : ARRAY_INITIAL_CAPACITY;

    assert(a->element_size > 0);
    
    size_t new_bytes;
    if(mul_overflow(&new_bytes, new_capacity, a->element_size)) return EOVERFLOW;

    void *new_data = realloc(a->data, new_bytes);
    if(!new_data) return ENOMEM;

    a->data = new_data;
    a->capacity = new_capacity;
    
    ARRAY_ASSERT(a);

    return 0;
}

static inline int array_shrink_fit(Array *a)
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
 * Validates preconditions for array_insert.
 *
 * @pre a != NULL
 * @pre value != NULL
 * @pre index <= a.size
 *
 * @return 0 on success, error code otherwise
 * 
 * @note No a.element_size check (ARRAY_ASSERT invariant check macro)
 */
static inline int
check_array_insert_entry(const Array *a, const void *restrict value, size_t index)
{
    if(!a || !value) return EINVAL;

    if(index > a->size) return EINVAL;
    
    return 0;
}

/**
 * Performs the core insertion: shifts tail right and copies value.
 *
 * @pre a != NULL && a.element_size > 0
 * @pre value != NULL
 * @pre index <= a.size
 * @pre enough capacity already reserved (a.capacity > a.size)
 *
 * @post On success:
 *          - elements [index+1, a->size] == old elements [index, a.size-1]
 *          - element at index == *value
 *          - a.size unchanged
 *
 * @post On failure:
 *          - array contents unchanged
 *
 * @return 0 on success, error code otherwise
 */
static inline int
do_insert(Array *restrict a, const void *restrict value, size_t index)
{
    size_t insert_offset;
    if(mul_overflow(&insert_offset, index, a->element_size)) return EOVERFLOW;

    size_t tail_offset;
    const size_t tail_count = (a->size - index); // safe: index <= a->size validated by caller
    if(mul_overflow(&tail_offset, tail_count, a->element_size)) return EOVERFLOW;

    const char *v = (const char *)value;
    const char *start = (const char *)a->data;

    size_t bytes;
    if(mul_overflow(&bytes, a->size, a->element_size)) return EOVERFLOW;

    const char *end = start + bytes;

    if(v < end && v >= start) return EINVAL;

    char *base = (char *)a->data;

    void *dst = base + insert_offset + a->element_size;
    void *src = base + insert_offset;

    memmove(dst, src, tail_offset);

    memcpy(src, value, a->element_size);

    return 0;
}

/**
 * Inserts an element at the specified index, shifting subsequent elements right.
 *
 * @pre a != NULL
 * @pre value != NULL
 * @pre index <= a.size
 * @pre a.element_size > 0
 *
 * @post On success:
 *       - a.size is increased by 1
 *       - element at position index equals *value
 *       - elements from index+1 to new size-1 equal old elements from index to old size-1
 *       - relative order of remaining elements is preserved
 *
 * @post On failure:
 *       - array size and contents remain unchanged
 *       - capacity may increase
 *
 * @return 0 on success, error code otherwise
 */
int
array_insert(Array *a, const void *restrict value, size_t index)
{
    ARRAY_ASSERT(a);

    int error; // contain error return from function.

    error = check_array_insert_entry(a, value, index);
    if(error) return error;

    error = array_reserve(a);
    if(error) return error;

    error = do_insert(a, value, index);
    if(error) return error;

    a->size++;

    ARRAY_ASSERT(a);

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

static inline int
check_array_erase_entry(Array *a, size_t index)
{
    if(!a) return EINVAL;

    if(index >= a->size) return EINVAL;

    return 0;
}

static inline int
do_erase(Array *a, size_t index)
{
    const size_t tail = (a->size - index);

    if(tail > 0)
    {
        size_t bytes;
        if(mul_overflow(&bytes, tail, a->element_size)) return EOVERFLOW;

        size_t dst_offset;
        if(mul_overflow(&dst_offset, index, a->element_size)) return EOVERFLOW;

        size_t src_offset;
        if(mul_overflow(&src_offset, (index + 1), a->element_size)) return EOVERFLOW;

        char *base = (char*)a->data;

        void * dst = base + dst_offset;
        void * src = base + src_offset;

        memmove(dst, src, bytes);
    }

    a->size--;

    return 0;
}
 
int array_erase(Array *a, size_t index)
{
    ARRAY_ASSERT(a);

    int error;

    error = check_array_erase_entry(a, index);
    if(error) return error;

    error = do_erase(a, index);
    if(error) return error;
    
    ARRAY_ASSERT(a);

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
        int error = array_reserve(a);
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
        int error = array_reserve(a);
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

const void *array_data(Array *a)
{
    return a ? a->data : NULL;
}
