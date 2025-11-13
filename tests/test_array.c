#include "../src/array.h"
#include <stdio.h>
#include <stdlib.h>

static void test_array_erase(void)
{
    printf("[ RUN ] test_array_erase\n");

    Array * array = array_new(sizeof(int));
    if(!array)
    {
        fprintf(stderr, "[ FAIL ] array_new() failed\n");
        exit(EXIT_FAILURE);
    }

    // Инициализация массива: 10 20 30 40 50
    int values[] = {10, 20, 30, 40, 50};
    for(int i = 0; i < 5; ++i)
    {
        if(array_push_back(array, &values[i]) != 0)
        {
            fprintf(stderr, "[ FAIL ] push_back failed at index %d\n", i);
            array_delete(array);
            exit(EXIT_FAILURE);
        }
    }

    printf("Initial array: ");
    for(size_t i = 0; i < array_size(array); ++i)
    {
        int v = 0;
        array_get(array, i, &v);
        printf("%d ", v);
    }
    printf("\n");

    // 1. Удалим элемент в середине (index = 2 → 30)
    if(array_erase(array, 2) != 0)
    {
        fprintf(stderr, "[ FAIL ] erase middle failed\n");
        array_delete(array);
        exit(EXIT_FAILURE);
    }

    printf("After erase(2): ");
    for(size_t i = 0; i < array_size(array); ++i)
    {
        int v = 0;
        array_get(array, i, &v);
        printf("%d ", v);
    }
    printf("\nExpected: 10 20 40 50\n");

    // 2. Удалим первый элемент (index = 0 → 10)
    if(array_erase(array, 0) != 0)
    {
        fprintf(stderr, "[ FAIL ] erase front failed\n");
        array_delete(array);
        exit(EXIT_FAILURE);
    }

    printf("After erase(0): ");
    for(size_t i = 0; i < array_size(array); ++i)
    {
        int v = 0;
        array_get(array, i, &v);
        printf("%d ", v);
    }
    printf("\nExpected: 20 40 50\n");

    // 3. Удалим последний элемент (index = size-1 → 50)
    if(array_erase(array, array_size(array) - 1) != 0)
    {
        fprintf(stderr, "[ FAIL ] erase back failed\n");
        array_delete(array);
        exit(EXIT_FAILURE);
    }

    printf("After erase(last): ");
    for(size_t i = 0; i < array_size(array); ++i)
    {
        int v = 0;
        array_get(array, i, &v);
        printf("%d ", v);
    }
    printf("\nExpected: 20 40\n");

    // Проверим соответствие ожидаемым значениям
    int expected[] = {20, 40};
    size_t n = sizeof(expected) / sizeof(expected[0]);
    for(size_t i = 0; i < n; ++i)
    {
        int out = 0;
        array_get(array, i, &out);
        if(out != expected[i])
        {
            fprintf(stderr, "[ FAIL ] array[%zu] = %d, expected %d\n", i, out, expected[i]);
            array_delete(array);
            exit(EXIT_FAILURE);
        }
    }

    printf("[ OK ] array_erase passed all checks\n");
    array_delete(array);
    printf("[ PASS ] test_array_erase completed\n\n");
}

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
        printf("[ FAIL ] expected empty array\n");
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
    printf("[ OK ] push_back/get passed (value=%d)\n", result);
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

static void test_array_insert(void)
{
    printf("[ RUN ] test_array_insert\n");

    Array * array = array_new(sizeof(int));
    if(!array)
    {
        fprintf(stderr, "[ FAIL ] array_new() failed\n");
        exit(EXIT_FAILURE);
    }

    // Заполним массив начальными значениями: 10, 20, 30
    int initial[] = {10, 20, 30};
    for(int i = 0; i < 3; ++i)
    {
        if(array_push_back(array, &initial[i]) != 0)
        {
            fprintf(stderr, "[ FAIL ] array_push_back failed at index %d\n", i);
            array_delete(array);
            exit(EXIT_FAILURE);
        }
    }

    printf("Initial array (size = %zu): ", array_size(array));
    for(size_t i = 0; i < array_size(array); ++i)
    {
        int v = 0;
        array_get(array, i, &v);
        printf("%d ", v);
    }
    printf("\n");

    // Вставим 5 в начало
    int v_front = 5;
    if(array_insert(array, 0, &v_front) != 0)
    {
        fprintf(stderr, "[ FAIL ] array_insert at front failed\n");
        array_delete(array);
        exit(EXIT_FAILURE);
    }

    // Вставим 25 в середину (между 20 и 30)
    int v_mid = 25;
    if(array_insert(array, 3, &v_mid) != 0)
    {
        fprintf(stderr, "[ FAIL ] array_insert in middle failed\n");
        array_delete(array);
        exit(EXIT_FAILURE);
    }

    // Вставим 40 в конец
    int v_back = 40;
    if(array_insert(array, array_size(array), &v_back) != 0)
    {
        fprintf(stderr, "[ FAIL ] array_insert at back failed\n");
        array_delete(array);
        exit(EXIT_FAILURE);
    }

    printf("After insertions (size = %zu): ", array_size(array));
    for(size_t i = 0; i < array_size(array); ++i)
    {
        int v = 0;
        array_get(array, i, &v);
        printf("%d ", v);
    }
    printf("\nExpected: 5 10 20 25 30 40\n");

    // Проверим правильность значений
    int expected[] = {5, 10, 20, 25, 30, 40};
    size_t n = sizeof(expected) / sizeof(expected[0]);
    for(size_t i = 0; i < n; ++i)
    {
        int out = 0;
        array_get(array, i, &out);
        if(out != expected[i])
        {
            fprintf(stderr, "[ FAIL ] array[%zu] = %d, expected %d\n", i, out,
                    expected[i]);
            array_delete(array);
            exit(EXIT_FAILURE);
        }
    }

    printf("[ OK ] array_insert passed all checks\n");
    array_delete(array);
    printf("[ PASS ] test_array_insert completed\n\n");
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
    test_array_insert();
    test_array_erase();
    printf("========== All Array Tests Passed ==========\n");
}
