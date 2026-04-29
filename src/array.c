#include "../include/array.h"

#include "../include/allocator.h"

#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static const size_t ARR_INIT_CAP = 8;
static const size_t ARR_GROWTH_FACTOR = 2;

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
array_reserve(Array *a, size_t min_capacity)
{
    if(!a) return EINVAL;

    assert(array_invariant_validation(a) == 0);

    if(min_capacity <= a->capacity) return 0; // enough capacity

    size_t new_capacity = a->capacity ? a->capacity : ARR_INIT_CAP;

    while(new_capacity < min_capacity)
    {
        if(mul_safe(new_capacity, ARR_GROWTH_FACTOR, &new_capacity))
        {
            return EOVERFLOW;
        }
    }

    size_t new_bytes;
    if(mul_safe(new_capacity, a->element_size, &new_bytes)) return EOVERFLOW;

    void *tmp = realloc(a->data, new_bytes);
    if(!tmp) return ENOMEM;

    a->data = tmp;
    a->capacity = new_capacity;

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

static inline int
do_insert(Array *restrict a, const void *restrict value, size_t index)
{
    size_t insert_offset;
    if(mul_safe(index, a->element_size, &insert_offset)) return EOVERFLOW;

    size_t tail_bytes;
    size_t tail_count = (a->size - index);
    if(mul_safe(tail_count, a->element_size, &tail_bytes)) return EOVERFLOW;

    char *base = (char *)a->data;

    void *dst = base + insert_offset + a->element_size;
    void *src = base + insert_offset;

    memmove(dst, src, tail_bytes);

    memcpy(src, value, a->element_size);

    return 0;
}

int
array_insert(Array *restrict a, const void *restrict value, size_t index)
{
    if(!a || !value || (index > a->size)) return EINVAL;

    int error; // contain error code return from function.

    error = array_reserve(a, a->size + 1);
    if(error) return error;

    error = do_insert(a, value, index);
    if(error) return error;

    error = array_size_safe_increment(a);
    if(error) return error;

    return 0;
}

static inline int
do_erase(Array *a, size_t index)
{
    size_t tail;
    if(sub_safe(a->size, (index + 1), &tail)) return EOVERFLOW;

    if(tail > 0)
    {
        size_t bytes;
        if(mul_safe(tail, a->element_size, &bytes)) return EOVERFLOW;

        size_t dst_off;
        if(mul_safe(index, a->element_size, &dst_off)) return EOVERFLOW;

        size_t src_off;
        if(mul_safe((index + 1), a->element_size, &src_off)) return EOVERFLOW;

        char *base = (char *)a->data;

        void *dst = base + dst_off;
        void *src = base + src_off;

        memmove(dst, src, bytes);
    }

    return 0;
}

int
array_erase(Array *a, size_t index)
{
    if(!a || (index >= a->size)) return EINVAL;

    int error; // contain error code return from function.

    error = do_erase(a, index);
    if(error) return error;

    error = array_size_safe_decrement(a);
    if(error) return error;

    return 0;
}

int
do_push_front(Array *restrict a, const void *restrict value)
{
    size_t bytes;
    if(mul_safe(a->size, a->element_size, &bytes)) return EOVERFLOW;

    char *base = (char *)a->data;

    void *dst = base + a->element_size;

    memmove(dst, base, bytes);
    memcpy(base, value, a->element_size);

    return 0;
}

int
array_push_front(Array *restrict a, const void *restrict value)
{
    if(!a || !value) return EINVAL;

    int error; // contain error code return from function.

    error = array_reserve(a, a->size + 1);
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

    error = array_reserve(a, a->size + 1);
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
