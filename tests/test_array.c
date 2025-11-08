#include <stdio.h>
#include <stdlib.h>
#include "../src/array.h"

static void test_array_init_and_delete(void)
{
    printf("[ RUN ] test_array_init_and_delete\n");

    Array *array = array_new(sizeof(int));
    if(!array)
    {
        fprintf(stderr, "[ FAIL ] array_new() returned NULL\n");
        exit(EXIT_FAILURE);
    }

    printf("Result: array_new() succeeded\n");

    array_delete(array);
    printf("[ OK ] array_delete() completed\n\n");
}

static void test_array_push_and_get(void)
{
    printf("[ RUN ] test_array_push_and_get\n");

    Array *array = array_new(sizeof(int));
    if(!array)
    {
        fprintf(stderr, "[ FAIL ] array_new() failed\n");
        exit(EXIT_FAILURE);
    }

    int value = 42;
    if(array_push_back(array, &value) != 0)
    {
        fprintf(stderr, "[ FAIL ] array_push_back() failed\n");
        array_delete(array);
        exit(EXIT_FAILURE);
    }

    int result = 0;
    if(array_get(array, 0, &result) != 0)
    {
        fprintf(stderr, "[ FAIL ] array_get() failed\n");
        array_delete(array);
        exit(EXIT_FAILURE);
    }

    if(result != value)
    {
        fprintf(stderr, "[ FAIL ] Expected %d, got %d\n", value, result);
        array_delete(array);
        exit(EXIT_FAILURE);
    }

    printf("Result: push_back/get passed (value=%d)\n", result);

    array_delete(array);
    printf("[ OK ] test_array_push_and_get passed\n\n");
}

static void test_array_set(void)
{
    printf("[ RUN ] test_array_set\n");

    Array *array = array_new(sizeof(int));
    if(!array)
    {
        fprintf(stderr, "[ FAIL ] array_new() failed\n");
        exit(EXIT_FAILURE);
    }

    int v1 = 10;
    int v2 = 20;
    array_push_back(array, &v1);
    array_push_back(array, &v1);

    if(array_set(array, 1, v2) != 0)
    {
        fprintf(stderr, "[ FAIL ] array_set() failed\n");
        array_delete(array);
        exit(EXIT_FAILURE);
    }

    int result = 0;
    array_get(array, 1, &result);

    if(result != v2)
    {
        fprintf(stderr, "[ FAIL ] Expected %d, got %d\n", v2, result);
        array_delete(array);
        exit(EXIT_FAILURE);
    }

    printf("Result: array_set/get passed (value=%d)\n", result);

    array_delete(array);
    printf("[ OK ] test_array_set passed\n\n");
}

/* Главная точка входа тестов */
void runArrayTests(void)
{
    printf("\n========== Running Array Tests ==========\n\n");

    test_array_init_and_delete();
    test_array_push_and_get();
    test_array_set();

    printf("========== All Array Tests Passed ==========\n");
}
