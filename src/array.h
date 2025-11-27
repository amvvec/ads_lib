#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <stdint.h>

typedef struct Array Array;

int array_init(Array *array, size_t element_size);
Array *array_new(size_t element_size);

int array_shrink_to_fit(Array *array);

int array_insert(Array *array, size_t index, const void *value);
int array_erase(Array *array, size_t index);
int array_push_front(Array *array, const void *value);
int array_push_back(Array *array, const void *value);
void array_pop_back(Array *array);
void array_pop_front(Array *array);

int array_get(const Array *array, size_t index, void *out_value);
int array_set(Array *array, size_t index, const void *value);

size_t array_size(const Array *array);
size_t array_capacity(const Array *array);

void array_free(Array *array);
void array_delete(Array *array);

#endif // !ARRAY_H
