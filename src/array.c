#include "../include/array.h"

#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static const size_t ARR_INIT_CAP = 8;

/*
@invariant:
    - a != NULL
    - a->data != NULL
    - a->element_size > 0
    - a->capacity > 0
    - a->size <= a->capacity
    - a->size / a->element_size <= SIZE_MAX
    - a->capacity / a->element_size <= SIZE_MAX
*/
struct Array
{
    size_t capacity;
    void *data;
    size_t element_size;
    size_t size;
};

int
array_invariant_validation(const Array *array)
{
    if(!array) return EINVAL;

    if(array->capacity == 0)
    {
        if(array->data != NULL) return EINVAL;
        if(array->size != 0) return EINVAL;
    }
    else
    {
        if(array->data == NULL) return EINVAL;
    }

    if(array->element_size == 0) return EINVAL;

    if(array->size > array->capacity) return EINVAL;

    return 0;
}

/*
@brief:
Perform size_t addition with overflow detection.

@note:
Low-level primitive. No parameter validation.

@pre:
    - out != NULL (caller must validate)
    - out may alias a or b

@post:
    On success:
        - return false
        - *out = a + b

    On failure:
        - return true
        - *out is unchanged
*/
bool
add_overflow(size_t a, size_t b, size_t *out)
{
    if(a > SIZE_MAX - b) return true;
    *out = a + b;
    return false;
}

/*
@brief:
Perform size_t subtraction with overflow detection.

@note:
Low-level primitive. No parameter validation.

@pre:
    - out != NULL (caller must validate)
    - out may alias a or b

@post:
    On success:
        - return false
        - *out = a - b

    On failure:
        - return true
        - *out is unchanged
*/
bool
sub_overflow(size_t a, size_t b, size_t *out)
{
    if(a < b) return true;
    *out = a - b;
    return false;
}

/*
@brief:
Perform size_t multiplication with overflow detection.

@note:
Low-level primitive. No parameter validation.

@pre:
    - out != NULL (caller must validate)
    - out may alias a or b

@post:
    On success:
        - return false
        - *out = a * b

    On failure:
        - return true
        - *out is unchanged
*/
bool
mul_overflow(size_t a, size_t b, size_t *out)
{
    if(a != 0 && b > SIZE_MAX / a) return true;
    *out = a * b;
    return false;
}

/*
@brief:
Safe wrapper over add_overflow.

@post:
    On success:
        - return 0
        - *out = a + b

    On invalid parameter:
        - return error code
        - *out is not accessed

    On overflow:
        - return error code
        - *out is not modified
*/
int
add_safe(size_t a, size_t b, size_t *out)
{
    if(!out) return EINVAL;

    size_t tmp;
    if(add_overflow(a, b, &tmp)) return EOVERFLOW;

    *out = tmp;

    return 0;
}

/*
@brief:
Safe wrapper over sub_overflow

@post:
    On success:
        - return 0
        - *out = a - b

    On invalid parameter:
        - return error code
        - *out is not accessed

    On overflow:
        - return error code
        - *out is not modified
*/
int
sub_safe(size_t a, size_t b, size_t *out)
{
    if(!out) return EINVAL;

    size_t tmp;
    if(sub_overflow(a, b, &tmp)) return EOVERFLOW;

    *out = tmp;

    return 0;
}

/*
@brief:
Safe wrapper over mul_overflow

@post:
    On success:
        - return 0
        - *out = a * b

    On invalid parameter:
        - return error code
        - *out is not accessed

    On overflow:
        - return error code
        - *out is not modified
*/
int
mul_safe(size_t a, size_t b, size_t *out)
{
    if(!out) return EINVAL;

    size_t tmp;
    if(mul_overflow(a, b, &tmp)) return EOVERFLOW;

    *out = tmp;

    return 0;
}

static inline int
array_self_insertion_safety(const Array *a, const void *value)
{
    size_t _bytes;
    if(mul_safe(a->size, a->element_size, &_bytes)) return EOVERFLOW;

    const char *v = value;
    const char *start = (const char *)a->data;
    const char *end = start + _bytes;

    return v < end && v >= start;
}

/*
@brief:
Initialize a dynamic array.

@note:
Returned object satisfies all Array invariants.

@pre:
    - element_size > 0

@post:
    On success (return != NULL):
        - a->capacity == ARR_INIT_CAP
        - a->data != NULL
        - a->element_size == element_size
        - a->size == 0
        - ARR_INIT_CAP * element_size did not overflow

    On failure: (return == NULL)
        - no memory is leaked
*/
Array *
array_init(size_t element_size)
{
    if(element_size == 0) return NULL;

    size_t new_bytes;
    if(mul_safe(ARR_INIT_CAP, element_size, &new_bytes)) return NULL;

    Array *tmp = calloc(1, sizeof(*tmp));
    if(!tmp) return NULL;

    void *data = malloc(new_bytes);
    if(!data)
    {
        free(tmp);
        return NULL;
    }

    tmp->data = data;
    tmp->element_size = element_size;
    tmp->size = 0;
    tmp->capacity = ARR_INIT_CAP;

    return tmp;
}

/*
@brief:
Delete the dynamic array and releases owned memory

@pre:
    - *a may be NULL
    - a may be NULL

@post:
    if a != NULL && *a != NULL:
        - (*a)->data is freed
        - *a is freed
        - *a is set to NULL

    if a == NULL && *a == NULL:
        - no action

@post:
No memory is leaked

@note:
Function is idempotent
*/
void
array_delete(Array **a)
{
    if(!a) return;

    if(*a)
    {
        free((*a)->data);
        free(*a);
        *a = NULL;
    }
}

int
array_reserve(Array *a)
{
    if(!a) return EINVAL;

    if(a->capacity > a->size) return 0; // enough capacity

    size_t capacity = (a->capacity == 0) ? ARR_INIT_CAP : (a->capacity * 2);

    size_t bytes;
    if(mul_safe(capacity, a->element_size, &bytes)) return EOVERFLOW;

    void *data = realloc(a->data, bytes);
    if(!data) ENOMEM;

    a->data = data;
    a->capacity = capacity;

    return 0;
}

static inline int
array_shrink_fit(Array *a)
{
    if(!a) return EINVAL;

    if(a->capacity == a->size) return 0; // enough memory

    if(a->size == 0)
    {
        free(a->data);

        a->data = NULL;
        a->capacity = 0;

        return 0;
    }

    size_t _bytes;
    if(mul_safe(a->size, a->element_size, &_bytes)) return EOVERFLOW;

    void *tmp = realloc(a->data, _bytes);
    if(!tmp) return ENOMEM;

    a->data = tmp;
    a->capacity = a->size;

    return 0;
}

int
array_size_safe_increment(Array *a)
{
    size_t new_size;
    if(add_safe(a->size, 1, &new_size)) return EOVERFLOW;

    a->size = new_size;

    return 0;
}

int
array_size_safe_decrement(Array *a)
{
    size_t new_size;
    if(sub_safe(a->size, 1, &new_size)) return EOVERFLOW;

    a->size = new_size;

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
 */
static inline int
check_array_insert_entry(const Array *a, const void *restrict value,
                         size_t index)
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
 *          - a contents unchanged
 *
 * @return 0 on success, error code otherwise
 */
static inline int
do_insert(Array *restrict a, const void *restrict value, size_t index)
{
    size_t insert_offset;
    if(mul_safe(index, a->element_size, &insert_offset)) return EOVERFLOW;

    size_t tail_offset;
    // safe: index <= a->size validated by caller
    const size_t tail_count = (a->size - index);

    if(mul_safe(tail_count, a->element_size, &tail_offset)) return EOVERFLOW;

    if(array_self_insertion_safety(a, value)) return EINVAL;

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
 *       - a size and contents remain unchanged
 *       - capacity may increase
 *
 * @return 0 on success, error code otherwise
 */
int
array_insert(Array *a, const void *restrict value, size_t index)
{
    int error; // contain error return from function.

    error = check_array_insert_entry(a, value, index);
    if(error) return error;

    error = array_reserve(a);
    if(error) return error;

    error = do_insert(a, value, index);
    if(error) return error;

    error = array_size_safe_increment(a);
    if(error) return error;

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
        if(mul_safe(tail, a->element_size, &bytes)) return EOVERFLOW;

        size_t dst_offset;
        if(mul_safe(index, a->element_size, &dst_offset)) return EOVERFLOW;

        size_t src_offset;
        if(mul_safe((index + 1), a->element_size, &src_offset))
            return EOVERFLOW;

        char *base = (char *)a->data;

        void *dst = base + dst_offset;
        void *src = base + src_offset;

        memmove(dst, src, bytes);
    }

    return 0;
}

int
array_erase(Array *a, size_t index)
{
    int error;

    error = check_array_erase_entry(a, index);
    if(error) return error;

    error = do_erase(a, index);
    if(error) return error;

    error = array_size_safe_decrement(a);
    if(error) return error;

    return 0;
}

static inline int
array_push_front_ensure_capacity(Array *a)
{
    if(a->capacity == a->size)
    {
        int error = array_reserve(a);
        if(error) return error;
    }

    return 0;
}

static inline int
do_push_front(Array *restrict a, const void *restrict value)
{
    size_t bytes;
    if(mul_safe(a->size, a->element_size, &bytes)) return EOVERFLOW;

    if(array_self_insertion_safety(a, value)) return EINVAL;

    char *base = (char *)a->data;

    void *dst = base + a->element_size;

    memmove(dst, base, bytes);

    memcpy(base, value, a->element_size);

    return 0;
}

/**
 * @brief Inserts value to first index, shifts rest right.
 *
 * @post On success:
 *
 * @post On failure:
 *
 * @return 0 on success, error code otherwise.
 */
int
array_push_front(Array *restrict a, const void *restrict value)
{
    if(!a || !value) return EINVAL;

    int error;

    error = array_push_front_ensure_capacity(a);
    if(error) return error;

    error = do_push_front(a, value);
    if(error) return error;

    error = array_size_safe_increment(a);
    if(error) return error;

    return 0;
}

static inline int
do_push_back(Array *a, const void *value)
{
    size_t dst_offset;
    if(mul_safe(a->size, a->element_size, &dst_offset)) return EOVERFLOW;

    char *base = (char *)a->data;
    void *dst = base + dst_offset;
    memcpy(dst, value, a->element_size);

    return 0;
}

int
array_push_back(Array *a, const void *value)
{
    if(!a || !value) return EINVAL;

    int error;

    error = array_reserve(a);
    if(error) return error;

    error = do_push_back(a, value);
    if(error) return error;

    error = array_size_safe_increment(a);
    if(error) return error;

    return 0;
}

void
array_pop_front(Array *a)
{
    if(!a) return;

    int error;

    size_t _bytes;
    if(mul_safe((a->size - 1), a->element_size, &_bytes)) return;

    char *base = (char *)a->data;
    void *dst = base;
    void *src = base + a->element_size;
    memmove(dst, src, _bytes);

    error = array_size_safe_decrement(a);
    if(error) return;
}

void
array_pop_back(Array *a)
{
    if(!a) return;

    int error;

    size_t bytes;
    if(mul_safe(a->size, a->element_size, &bytes)) return;

    char *base = (char *)a->data;
    void *dst = base + bytes;
    memset(dst, 0, a->element_size);

    error = array_size_safe_decrement(a);
    if(error) return;
}

int
array_get(const Array *a, size_t index, void *value)
{
    if(!a || !value) return EINVAL;
    if(index >= a->size) return EINVAL;

    const char *base = (const char *)a->data;

    memcpy(value, base + index * a->element_size, a->element_size);

    return 0;
}

int
array_set(Array *a, size_t index, const void *value)
{
    if(!a || !value) return EINVAL;
    if(index >= a->size) return EINVAL;

    char *base = (char *)a->data;

    void *dst = base + index * a->element_size;

    memcpy(dst, value, a->element_size);

    return 0;
}

size_t
array_size(const Array *a)
{
    return a ? a->size : 0;
}

size_t
array_capacity(const Array *a)
{
    return a ? a->capacity : 0;
}
