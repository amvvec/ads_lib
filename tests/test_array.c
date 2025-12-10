#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/array.h"

static void test_array_init_basic(void)
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

static void test_array_insert_basic(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    const int v = 1;
    array_insert(a, &v, 0);

    const size_t s = array_size(a);
    assert(s == 1);

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

    assert(array_insert(a, NULL, 0) != 0);

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
    test_array_init_basic();
    test_array_init_zero_size();
    test_array_init_large_size();
    test_array_delete_null();
    test_array_delete_nonempty();
    test_array_delete_double();
    test_array_insert_basic();
    test_array_erase_basic();
    test_array_insert_null_array();
    test_array_insert_null_value();
}
