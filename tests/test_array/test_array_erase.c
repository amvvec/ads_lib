#include <assert.h>

#include "../src/array.h"

static void test_array_erase_back(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    for(int i = 0; i < 10; ++i)
        assert(array_push_back(a, &i) == 0);

    assert(array_size(a) == 10);

    size_t cap = array_capacity(a);

    assert(array_erase(a, 9) == 0);
    assert(array_size(a) == 9);
    int v = -1;
    assert(array_get(a, 8, &v) == 0);
    assert(v == 8);

    assert(array_erase(a, 8) == 0);
    assert(array_size(a) == 8);

    assert(array_capacity(a) == cap); // no shrink

    array_delete(&a);
}

static void test_array_erase_middle(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    for(int i = 0; i < 7; ++i)
        assert(array_push_back(a, &i) == 0);

    assert(array_erase(a, 3) == 0);
    assert(array_size(a) == 6);

    int expected[] = {0, 1, 2, 4, 5, 6};
    for(size_t i = 0; i < 6; ++i)
    {
        int v = -1;
        assert(array_get(a, i, &v) == 0);
        assert(v == expected[i]);
    }

    array_delete(&a);
}

static void test_array_erase_front(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    const int values[] = {10, 20, 30, 40};
    const int n = sizeof(values) / sizeof(values[0]);

    for(int i = 0; i < n; ++i)
    {
        assert(array_push_back(a, &values[i]) == 0);
    }

    assert(array_erase(a, 0) == 0);
    assert(array_size(a) == 3);

    int v = -1;
    assert(array_get(a, 0, &v) == 0);

    assert(v == 20);

    array_delete(&a);
}

void run_array_erase_tests(void)
{
    test_array_erase_back();
    test_array_erase_middle();
    test_array_erase_front();
}
