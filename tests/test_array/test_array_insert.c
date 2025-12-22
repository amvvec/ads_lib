#include <assert.h>

#include "../src/array.h"

static void assert_array_invariants(const Array * a)
{
    assert(a != NULL);
    assert(array_capacity(a) >= array_size(a));
}

static void test_array_insert_back(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    // Empty array initially
    assert(array_size(a) == 0);
    assert(array_capacity(a) >= 1);
    assert(array_size(a) <= array_capacity(a));

    size_t initial_capacity = array_capacity(a);

    // Insert 20 elements at the back using insert at current size
    for(int i = 0; i < 20; ++i)
    {
        assert(array_insert(a, &i, array_size(a)) == 0);
        assert(array_size(a) == (size_t)i + 1);
    }

    // Verify preserved order and values after multiple reallocations
    for(int i = 0; i < 20; ++i)
    {
        int value = -1;
        assert(array_get(a, (size_t)i, &value) == 0);
        assert(value == i);
    }

    // Capacity must have grown beyond initial value
    assert(array_capacity(a) > initial_capacity);

    array_delete(a);
}

static void test_array_insert_front(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    // Empty array initially
    assert(array_size(a) == 0);
    assert(array_capacity(a) >= 1);
    assert(array_size(a) <= array_capacity(a));

    size_t initial_capacity = array_capacity(a);

    // Values to insert at front (order in array will be reversed)
    const int values[] = {5, 4, 3, 2, 1};
    const int n = sizeof(values) / sizeof(values[0]);

    // Repeatedly insert at position 0
    for(int i = 0; i < n; ++i)
    {
        assert(array_insert(a, &values[i], 0) == 0);
        assert(array_size(a) == (size_t)(i + 1));
    }

    // Verify reversed order after all front insertions
    for(int i = 0; i < n; ++i)
    {
        int out = 0;
        assert(array_get(a, (size_t)i, &out) == 0);
        assert(out == values[n - i - 1]);
    }

    // Capacity should not shrink (at least initial)
    assert(array_capacity(a) >= initial_capacity);

    array_delete(a);
}

void run_array_insert_tests(void)
{
    test_array_insert_back();
    test_array_insert_front();
}
