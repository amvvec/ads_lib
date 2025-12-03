#include "vector.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/** @brief Initializes a new Vector with the specified initial capacity.
 *  @param initial_capacity The initial number of elements the vector can hold.
 *  @return Pointer to the new Vector, or NULL if initialization fails (invalid
 * capacity or memory allocation error).
 *  @note Sets size to 0 and capacity to the given initial_capacity. O(1) time
 * complexity.
 */
Vector *vector_init(int initial_capacity)
{
    if(initial_capacity <= 0)
    {
        fprintf(stderr, "Invalid initial capacity\n");
        return NULL;
    }
    Vector *vec = (Vector *)malloc(sizeof(Vector));
    if(!vec)
    {
        fprintf(stderr, "Memory allocation failed for vector\n");
        return NULL;
    }
    vec->data = (int *)malloc(sizeof(initial_capacity * sizeof(int)));
    if(!vec->data)
    {
        fprintf(stderr, "Memory allocation failed for data array\n");
        free(vec);
        vec = NULL;
        return NULL;
    }

    vec->size = 0;
    vec->capacity = initial_capacity;
    return vec;
}

/** @brief Adds an element to the end of the Vector.
 *  @param vec Pointer to the Vector.
 *  @param value The integer value to add.
 *  @note Resizes the vector by doubling capacity if full. O(1) amortized time
 * complexity.
 */
void vector_push_back(Vector *vec, int value)
{
    if(!vec)
    {
        fprintf(stderr, "Null vector pointer\n");
        return;
    }
    if(vec->size == vec->capacity)
    {
        if(vec->capacity > INT_MAX / 2)
        {
            fprintf(stderr, "Capacity overflowed\n");
            return;
        }
        vec->capacity *= 2;
        int *new_data = (int *)realloc(vec->data, (size_t)vec->capacity * sizeof(int));
        if(!new_data)
        {
            fprintf(stderr, "Reallocation failed\n");
            return;
        }
        vec->data = new_data;
    }
    vec->data[vec->size++] = value;
}

/** @brief Adds an element to the end of the Vector.
 *  @param vec Pointer to the Vector.
 *  @param value The integer value to add.
 *  @note Resizes the vector by doubling capacity if full. O(1) amortized time
 * complexity.
 */
void vector_pop_back(Vector *vec)
{
    if(!vec || vec->size <= 0)
    {
        fprintf(stderr, "Cannot pop from empty or null vector\n");
        return;
    }
    vec->size--;
}

/** @brief Retrieves the element at the specified index.
 *  @param vec Pointer to the Vector.
 *  @param index The index of the element to retrieve.
 *  @return The integer value at the specified index, or 0 if index is invalid
 * or vector is NULL.
 *  @note No bounds checking beyond NULL and size comparison. O(1) time
 * complexity.
 */
int vector_get(Vector *vec, int index)
{
    if(!vec || index < 0 || index >= vec->size)
    {
        fprintf(stderr, "Invalid index or null vector\n");
        return 0;
    }
    return vec->data[index];
}

/** @brief Frees the memory allocated for the Vector.
 *  @param vec Pointer to the Vector.
 *  @note Does nothing if the vector is NULL. O(1) time complexity.
 */
void vector_free(Vector *vec)
{
    if(!vec)
    {
        fprintf(stderr, "Null vector pointer\n");
        return;
    }
    free(vec->data);
    free(vec);
}
