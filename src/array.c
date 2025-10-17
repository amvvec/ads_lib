#include "array.h"

int node_array_init(NodeArray* array)
{
    if(!array)
    {
        return EINVAL;
    }
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
    return 0;
}
