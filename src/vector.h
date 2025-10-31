#ifndef VECTOR_H
#define VECTOR_H

/** @struct Vector
 *  @brief A dynamic array implementation with adjustable capacity.
 *  @var Vector::data Pointer to the integer array storing the elements.
 *  @var Vector::size Current number of elements in the vector.
 *  @var Vector::capacity Maximum number of elements the vector can hold before
 * resizing.
 */
typedef struct
{
    int * data;   /**< Pointer to the integer array storing the elements. */
    int size;     /**< Current number of elements in the vector. */
    int capacity; /**< Maximum number of elements the vector can hold before
                     resizing. */
} Vector;

Vector * vector_init(int initial_capacity);
void vector_push_back(Vector * vec, int value);
void vector_pop_back(Vector * vec);
int vector_get(Vector * vec, int index);
void vector_free(Vector * vec);

#endif // !VECTOR_H
