#include "vector.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Vector
{
    void *data;
    size_t size;
    size_t capacity;
} Vector;

/**
 * @brief Initializes a new Vector with the specified initial capacity.
 *
 * @pre initial_capacity != 0
 *
 * @return Pointer to the new Vector, or NULL if initialization fails.
 */
Vector *vector_init(size_t initial_capacity)
{
    if(initial_capacity == 0)
    {
        return NULL;
    }

    Vector *v = malloc(sizeof(*v));
    if(!v)
    {
        return NULL;
    }

    v->data = malloc(sizeof(int) * initial_capacity);
    if(!v->data)
    {
        free(v);
        return NULL;
    }

    v->size = 0;
    v->capacity = initial_capacity;

    return v;
}

int vector_push_back(Vector *v, const void *value)
{
    if(!v || !value)
    {
        return EINVAL;
    }

    if(v->capacity <= v->size)
    {
        // TODO: capacity grow
    }

    char *base = (char *)v->data;

    size_t offset = v->size * sizeof(int);

    memcpy(base + offset, value, sizeof(int));

    v->size++;

    return 0;
}

/** @brief Adds an element to the end of the Vector.
 *  @param v Pointer to the Vector.
 *  @param value The integer value to add.
 *  @note Resizes the vector by doubling capacity if full. O(1) amortized time
 * complexity.
 */
void vector_pop_back(Vector *v)
{
    if(!v || v->size <= 0)
    {
        fprintf(stderr, "Cannot pop from empty or null vector\n");
        return;
    }
    v->size--;
}

/** @brief Frees the memory allocated for the Vector.
 *  @param v Pointer to the Vector.
 *  @note Does nothing if the vector is NULL. O(1) time complexity.
 */
void vector_free(Vector *v)
{
    if(!v)
    {
        fprintf(stderr, "Null vector pointer\n");
        return;
    }
    free(v->data);
    free(v);
}
