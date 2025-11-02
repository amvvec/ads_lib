#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <stdint.h>

typedef struct ArrayNode ArrayNode;
typedef struct Array Array;

int array_init(Array * a, size_t element_size);
Array * array_new(size_t element_size);

int array_grow_to(Array * a, size_t start_capacity);

void array_free(Array * a);
void array_delete(Array * a);

#endif // !ARRAY_H
