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

static void test_array_new_large_size(void)
{
    printf("\n[ RUN ] test_array_new_large_size\n");

    size_t size = (((size_t)-1) / 2);

    Array * array = array_new(size);
    assert(array == NULL);

    printf("[ PASS ] test_array_new_large_size\n\n");
}

static void test_array_delete_null(void)
{
    printf("[ RUN ] test_array_delete_null\n");

    array_delete(NULL);

    printf("[ PASS ] test_array_delete_null\n\n");
}

static void test_array_delete_nonempty(void)
{
    printf("[ RUN ] test_array_delete_nonempty\n");
    
    Array * a = array_new(sizeof(int));
    assert(a != NULL);

    array_delete(a);

    printf("[ PASS ] test_array_delete_nonempty\n\n");
}

void run_array_tests(void)
{
    test_array_new();
    test_array_new_zero_size();
    test_array_new_large_size();
    test_array_delete_null();
    test_array_delete_nonempty();
}
