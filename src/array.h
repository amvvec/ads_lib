#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <stdint.h>

typedef struct ArrayNode ArrayNode;
typedef struct Array Array;

int array_init(Array * array, size_t element_size);
Array * array_new(size_t element_size);

int array_grow_to(Array * array, size_t start_capacity);

void array_free(Array * array);
void array_delete(Array * array);

int array_push_front(Array * array, const void * value);
int array_push_back(Array * array, const void * value);
void array_pop_back(Array * array);

int array_get(const Array * array, size_t index, int * out_value);
int array_set(Array * array, size_t index, int value);
size_t array_size(const Array * array);
size_t array_capacity(const Array * array);

#endif // !ARRAY_H
