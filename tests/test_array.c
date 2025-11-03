#include <stdio.h>
#include <stdlib.h>

#include "../src/array.h"

void runArrayTests(void)
{
    printf("\n[ RUN ] Array Tests\n\n");
    // Test 1: Initialization
    printf("Test 1: Initialize array\n");
    Array * array = array_new(sizeof(int));
    if(!array)
    {
        fprintf(stderr, "Result: Initialization failed\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Result: Initialized successfully\n");
    }
    array_delete(array);
    printf("\n[ PASS ] Array Tests\n");
}
