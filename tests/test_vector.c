#include "../src/vector.h"
#include <stdio.h>

void runVectorTests()
{
    // Test 1: Initialization
    printf("Test 1: Initialize Vector with capacity 2\n");
    Vector* vec = vector_init(2);
    if(vec && vec->capacity == 2 && vec->size == 0)
    {
        printf("Result: Initialized successfully\n");
    }
    else
    {
        printf("Result: Initialization failed\n");
    }

    // Test 2: Push back
    printf("Test 2: Push back 1, 2, 3\n");
    vector_push_back(vec, 1);
    vector_push_back(vec, 2);
    vector_push_back(vec, 3); // Should trigger resize
    if(vec->size == 3)
    {
        printf("Result: Size is %d, get(0)=%d\n", vec->size,
               vector_get(vec, 0));
    }
    else
    {
        printf("Result: Push back failed\n");
    }

    // Test 3: Pop back
    printf("Test 3: Pop back\n");
    vector_pop_back(vec);
    if(vec->size == 2)
    {
        printf("Result: Size is %d\n", vec->size);
    }
    else
    {
        printf("Result: Pop back failed\n");
    }

    // Test 4: Get
    printf("Test 4: Get element at index 1\n");
    int value = vector_get(vec, 1);
    if(value == 2)
    {
        printf("Result: Got %d\n", value);
    }
    else
    {
        printf("Result: Get failed\n");
    }

    // Test 5: Free
    printf("Test 5: Free Vector");
    vector_free(vec);
    printf("Result: Freed\n");
}
