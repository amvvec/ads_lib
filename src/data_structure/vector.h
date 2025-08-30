#ifndef VECTOR_H
#define VECTOR_H

typedef struct
{
    int * data;
    int size;
    int capacity;
} Vector;

Vector * vector_init(int initial_capacity);
void vector_push_back(Vector * vec, int value);
void vector_pop_back(Vector * vec);
int vector_get(Vector * vec, int index);
void vector_free(Vector * vec);

#endif // !VECTOR_H
