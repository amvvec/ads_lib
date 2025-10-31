#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <stdint.h>

typedef struct ArrayNode ArrayNode;
typedef struct Array Array;

int array_init(Array * a, size_t element_size);
Array * array_new(size_t element_size);

int array_grow_to(Array * a, size_t start_capacity);
int array_reserve(void * array_struct, size_t * capacity_field,
                  void ** data_field, size_t elem_size, size_t n);

void array_free(Array * a);
void array_delete(Array * a);

#endif // !ARRAY_H
