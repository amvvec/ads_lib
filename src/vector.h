#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct Vector Vector;

Vector *vector_init(size_t initial_capacity);

int vector_push_back(Vector *v, const void *value);

void vector_pop_back(Vector *vec);
int vector_get(Vector *vec, int index);
void vector_free(Vector *vec);

#endif // !VECTOR_H
