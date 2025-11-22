#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/array.h"

static void test_array_new(void)
{
    printf("\n[ RUN ] test_array_new\n");

    Array * array = array_new(sizeof(int));
    assert(array != NULL);
    array_delete(array);

    printf("[ PASS ] test_array_new\n\n");
}

void run_array_tests(void)
{
    test_array_new();
    test_array_new_zero_size();
    test_array_new_large_size();
}
