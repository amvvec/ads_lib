#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <stdint.h>

typedef struct Array Array;

Array *array_init(size_t element_size);
void array_delete(Array **a);

int array_insert(Array *array, const void *value, size_t index);
int array_erase(Array *array, size_t index);

int array_push_front(Array *array, const void *value);
int array_push_back(Array *array, const void *value);

void array_pop_front(Array *array);
void array_pop_back(Array *array);

int array_set(Array *array, size_t index, const void *value);
int array_get(const Array *array, size_t index, void *out_value);

size_t array_capacity(const Array *array);
size_t array_size(const Array *array);

#endif // !ARRAY_H
