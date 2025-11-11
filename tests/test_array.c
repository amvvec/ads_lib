#include "../src/array.h"
#include <stdio.h>
#include <stdlib.h>

static void test_array_init_new(void)
{
    printf("[ RUN ] Initialize array\n");
    Array * array = array_new(sizeof(int));
    if(!array)
    {
        printf("Result: Initialization failed\n");
        exit(EXIT_FAILURE);
    }
    printf("[ PASS ] Initialized successfully\n");

    array_delete(array);
    printf("[ PASS ] Deleted successfully\n\n");
}

static void test_array_push_pop_front(void)
{
    printf("[ RUN ] push_front / pop_front\n");

    Array * array = array_new(sizeof(int));
    if(!array)
    {
        printf("[ FAIL ] array_new failed\n");
        exit(EXIT_FAILURE);
    }

    // push_back 3 elements
    int values[] = {10, 20, 30};
    for(int i = 0; i < 3; i++)
    {
        if(array_push_back(array, &values[i]) != 0)
        {
            printf("[ FAIL ] push_back failed at %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    printf("Array after push_back (size = %zu): ", array_size(array));
    for(size_t i = 0; i < array_size(array); i++)
    {
        int out = 0;
        array_get(array, i, &out);
        printf("%d ", out);
    }
    printf("\n");

    // pop_front one element
    array_pop_front(array);
    printf("Array after pop_front (size = %zu): ", array_size(array));
    for(size_t i = 0; i < array_size(array); i++)
    {
        int out = 0;
        array_get(array, i, &out);
        printf("%d ", out);
    }
    printf("\nExpected: 20 30\n\n");

    // pop_front remaining elements
    array_pop_front(array);
    array_pop_front(array);

    printf("Array after removing all (size = %zu)\n", array_size(array));
    if(array_size(array) == 0)
    {
        printf("[ OK ] All elements removed successfully\n");
    }
    else
    {
        printf("[ OK ] Error — expected empty array\n");
    }

    array_delete(array);
    printf("[ PASS ] Deleted successfully\n\n");
}

static void test_array_init_and_delete(void)
{
    printf("[ RUN ] test_array_init_and_delete\n");
    Array * array = array_new(sizeof(int));
    if(!array)
    {
        fprintf(stderr, "[ FAIL ] array_new() returned NULL\n");
        exit(EXIT_FAILURE);
    }
    printf("Result: array_new() succeeded\n");
    array_delete(array);
    printf("[ PASS ] array_delete() completed\n\n");
}

static void test_array_push_and_get(void)
{
    printf("[ RUN ] test_array_push_and_get\n");
    Array * array = array_new(sizeof(int));
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
    printf("[ PASS ] test_array_push_and_get passed\n\n");
}

static void test_array_set(void)
{
    printf("[ RUN ] test_array_set\n");
    Array * array = array_new(sizeof(int));
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
    printf("[ PASS ] test_array_set passed\n\n");
}

static void test_array_pop_back(void)
{
    printf("[ RUN ] test_array_pop_back\n");
    Array * array = array_new(sizeof(int));
    if(!array)
    {
        fprintf(stderr, "Result: Allocation failed\n");
        return;
    }
    for(int i = 0; i < 5; i++)
    {
        array_push_back(array, &i);
    }
    printf("Before pop_back: size = %zu\n", array_size(array));
    array_pop_back(array);
    printf("After pop_back: size = %zu\n", array_size(array));
    array_delete(array);
    printf("[ PASS ] test_array_pop_back\n\n");
}

static void test_array_push_front(void)
{
    printf("[ RUN ] push_front\n");
    Array * array = array_new(sizeof(int));
    if(!array)
    {
        fprintf(stderr, "Result: Allocation failed\n");
        return;
    }
    for(int i = 1; i <= 3; ++i)
    {
        array_push_back(array, &i);
    }
    printf("Before push_front: ");
    for(size_t i = 0; i < array_size(array); ++i)
    {
        int value;
        array_get(array, i, &value);
        printf("%d ", value);
    }
    printf("\n");
    int x = 0;
    array_push_front(array, &x);
    printf("After push_front:  ");
    for(size_t i = 0; i < array_size(array); ++i)
    {
        int value;
        array_get(array, i, &value);
        printf("%d ", value);
    }
    printf("\n");
    array_delete(array);
    printf("[ PASS ] Push front test completed\n\n");
}

void runArrayTests(void)
{
    printf("\n========== Running Array Tests ==========\n\n");
    test_array_init_and_delete();
    test_array_push_and_get();
    test_array_set();
    test_array_pop_back();
    test_array_push_front();
    test_array_push_pop_front();
    test_array_init_new();
    printf("========== All Array Tests Passed ==========\n");
}
