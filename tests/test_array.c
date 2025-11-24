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

    printf("[ PASS ] test_array_new\n");
}

static void test_array_new_zero_size(void)
{
    printf("\n[ RUN ] test_array_new_zero_size\n");

    Array * array = array_new(0);
    assert(array == NULL);

    printf("[ PASS ] test_array_new_zero_size\n");
}

void run_array_tests(void)
{
    test_array_new();
    test_array_new_zero_size();
}
