#include "vector.h"

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

Vector * vector_init(int initial_capacity)
{
    if(initial_capacity <= 0)
    {
        fprintf(stderr, "Invalid initial capacity\n");
        return NULL;
    }
    Vector * vec = (Vector *)malloc(sizeof(Vector));
    if(!vec)
    {
        fprintf(stderr, "Memory allocation failed for vector\n");
        return NULL;
    }
    vec->data = (int *)
        malloc(sizeof(initial_capacity * sizeof(int)));
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


void vector_push_back(Vector * vec, int value)
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
        int * new_data = (int *)
            realloc(vec->data, vec->capacity * sizeof(int));
        if(!new_data)
        {
            fprintf(stderr, "Reallocation failed\n");
            return;
        }
        vec->data = new_data;
    }
    vec->data[vec->size++] = value;
}

void vector_pop_back(Vector * vec)
{
    if(!vec || vec->size <= 0)
    {
        fprintf(stderr, "Cannot pop from empty or null vector\n");
        return;
    }
    vec->size--;
}

int vector_get(Vector * vec, int index)
{
    if(!vec || index < 0 || index >= vec->size)
    {
        fprintf(stderr, "Invalid index or null vector\n");
        return 0;
    }
    return vec->data[index];
}

void vector_free(Vector * vec)
{
    if(!vec)
    {
        fprintf(stderr, "Null vector pointer\n");
        return;
    }
    free(vec->data);
    free(vec);
}
