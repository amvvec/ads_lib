#include <stdlib.h>
#include "vector.h"

Vector * vector_init(int initial_capacity)
{
    Vector * vec = (Vector *)malloc(sizeof(Vector));
    vec->data = (int *)
        malloc(sizeof(initial_capacity * sizeof(int)));
    vec->size = 0;
    vec->capacity = initial_capacity;
    return vec;
}


void vector_push_back(Vector * vec, int value)
{
    if(vec->size == vec->capacity)
    {
        vec->capacity *= 2;
        vec->data = (int *)
            realloc(vec->data, vec->capacity * sizeof(int));
    }
    vec->data[vec->size++] = value;
}

void vector_pop_back(Vector * vec)
{
    if(vec->size > 0)
    {
        vec->size--;
    }
}

int vector_get(Vector * vec, int index)
{
    return vec->data[index];
}

void vector_free(Vector * vec)
{
    free(vec->data);
    free(vec);
}
