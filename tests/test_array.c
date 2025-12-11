#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/array.h"

static void test_array_init_and_delete(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);
    array_delete(a);
    a = NULL;
}

static void test_array_init_zero_size(void)
{
    Array *a = array_init(0);

    assert(a == NULL);
    array_delete(a);
    a = NULL;
}

static void test_array_init_large_size(void)
{
    size_t size = (((size_t)-1) / 2);

    Array *a = array_init(size);

    assert(a == NULL);

    array_delete(a);
    a = NULL;
}

static void test_array_delete_null(void)
{
    array_delete(NULL);
}

static void test_array_delete_nonempty(void)
{
    Array *a = array_init(sizeof(int));

    assert(a != NULL);

    array_delete(a);
    a = NULL;
}

static void test_array_delete_double(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    array_delete(a);
    a = NULL;
    array_delete(a);
}

static void test_array_insert_empty_array(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    int value1 = 1;
    size_t out = 0u;

    assert(array_insert(a, &value1, 0) == 0);
    assert(array_size(a) == 1);
    assert(array_get(a, 0, &out) == 0);
    assert(out == 1);

    array_delete(a);
    a = NULL;
}

static void test_array_insert_null_array(void)
{
    int v = 1;

    assert(array_insert(NULL, &v, 0) != 0); // NULL array
}

static void test_array_insert_null_value(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    assert(array_insert(a, NULL, 0) != 0); // NULL value

    array_delete(a);
    a = NULL;
}

static void test_array_insert_invalid_index(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    int v = 1;

    assert(array_insert(a, &v, 1) != 0); // index > size (array is empty)
    assert(array_insert(a, &v, 0) == 0);

    array_delete(a);
    a = NULL;
}

static void test_array_insert_back_preserve_order(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    int value1 = 1;
    int out1 = 0;

    assert(array_insert(a, &value1, 0) == 0);
    assert(array_size(a) == 1);
    assert(array_get(a, 0, &out1) == 0);
    assert(out1 == 1);

    int value2 = 2;
    int out2 = 0;

    assert(array_insert(a, &value2, 1) == 0);
    assert(array_size(a) == 2);
    assert(array_get(a, 0, &out1) == 0);
    assert(out1 == 1);
    assert(array_get(a, 1, &out2) == 0);
    assert(out2 == 2);

    array_delete(a);
    a = NULL;
}

static void test_array_erase_basic(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    const int v = 1;
    array_insert(a, &v, 0);
    array_erase(a, 0);

    const size_t s = array_size(a);
    assert(s == 0);

    array_delete(a);
    a = NULL;
}

void run_array_tests(void)
{
    test_array_init_and_delete();
    test_array_init_zero_size();
    test_array_init_large_size();
    test_array_delete_null();
    test_array_delete_nonempty();
    test_array_delete_double();
    test_array_insert_empty_array();
    test_array_insert_null_array();
    test_array_insert_null_value();
    test_array_insert_invalid_index();
    test_array_insert_back_preserve_order();
    test_array_erase_basic();
}
