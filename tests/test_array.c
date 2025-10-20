#include <stdio.h>
#include <stdlib.h>

#include "../src/array.h"

void runArrayTests(void)
{
    printf("\n[ RUN ] Array Tests\n\n");
    // Test 1: Initialization
    printf("Test 1: Initialize array\n");
    Array* array = node_array_new();
    if(!array)
    {
        fprintf(stderr, "Result: Initialization failed\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Result: Initialized successfully\n");
    }
    node_array_free(array);
    printf("\n[ PASS ] Array Tests\n");
}
