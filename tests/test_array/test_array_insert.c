#include <assert.h>

#include "../src/array.h"

static void test_array_insert_empty_at_zero(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);
    assert(array_size(a) == 0);

    int val = 42;
    assert(array_insert(a, &val, 0) == 0);
    assert(array_size(a) == 1);

    int got = 0;
    assert(array_get(a, 0, &got) == 0);
    assert(got == 42);

    assert(array_capacity(a) >= 1);

    array_delete(&a);
}

static void test_array_insert_at_end_on_empty(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);
    assert(array_size(a) == 0);

    int val = 777;
    assert(array_insert(a, &val, array_size(a)) == 0);

    assert(array_size(a) == 1);
    int got = 0;
    assert(array_get(a, 0, &got) == 0);
    assert(got == 777);

    array_delete(&a);
}

static void test_array_insert_single_element_middle(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    int vals[] = {10, 30};
    assert(array_push_back(a, &vals[0]) == 0);
    assert(array_push_back(a, &vals[1]) == 0);
    assert(array_size(a) == 2);

    int insert_val = 20;
    assert(array_insert(a, &insert_val, 1) == 0);

    assert(array_size(a) == 3);

    int expected[] = {10, 20, 30};
    for(size_t i = 0; i < 3; ++i)
    {
        int got = 0;
        assert(array_get(a, i, &got) == 0);
        assert(got == expected[i]);
    }

    array_delete(&a);
}

static void test_array_insert_front_on_nonempty(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    int vals[] = {20, 30, 40};
    for(size_t i = 0; i < 3; ++i)
    {
        assert(array_push_back(a, &vals[i]) == 0);
    }
    assert(array_size(a) == 3);

    int insert_val = 10;
    assert(array_insert(a, &insert_val, 0) == 0);

    assert(array_size(a) == 4);

    int expected[] = {10, 20, 30, 40};
    for(size_t i = 0; i < 4; ++i)
    {
        int got = 0;
        assert(array_get(a, i, &got) == 0);
        assert(got == expected[i]);
    }

    array_delete(&a);
}

static void test_array_insert_back_equivalent_to_push(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);
    assert(array_size(a) == 0);

    // fill using array_insert at end (should behave exactly as push_back)
    for(int i = 0; i < 25; ++i)
    {
        assert(array_insert(a, &i, array_size(a)) == 0);
    }

    assert(array_size(a) == 25);

    // verify identical content as if push_back was used
    for(int i = 0; i < 25; ++i)
    {
        int val = -1;
        assert(array_get(a, (size_t)i, &val) == 0);
        assert(val == i);
    }

    array_delete(&a);
}

static void test_array_insert_middle_single(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    // initial data: 0 1 2 3 4 5 6 7 8 9
    for(int i = 0; i < 10; ++i)
    {
        assert(array_push_back(a, &i) == 0);
    }

    size_t old_capacity = array_capacity(a);

    // insert 999 exactly in the middle (index 5)
    int insert_val = 999;
    assert(array_insert(a, &insert_val, 5) == 0);

    assert(array_size(a) == 11);
    assert(array_capacity(a) >= old_capacity);

    // expected: 0 1 2 3 4 999 5 6 7 8 9
    const int expected[] = {0, 1, 2, 3, 4, 999, 5, 6, 7, 8, 9};
    for(size_t i = 0; i < 11; ++i)
    {
        int val = -1;
        assert(array_get(a, i, &val) == 0);
        assert(val == expected[i]);
    }

    array_delete(&a);
}

static void test_array_insert_at_last_position(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    // fill with 1...5
    for(int i = 1; i <= 5; ++i)
    {
        assert(array_push_back(a, &i) == 0);
    }

    int val = 999;
    // insert right before end (at position 4, becomes new last-1)
    assert(array_insert(a, &val, 4) == 0);

    assert(array_size(a) == 6);

    const int expected[] = {1, 2, 3, 4, 999, 5};
    for(size_t i = 0; i < 6; ++i)
    {
        int got = 0;
        assert(array_get(a, i, &got) == 0);
        assert(got == expected[i]);
    }

    array_delete(&a);
}

static void test_array_insert_when_full_capacity(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    size_t cap = array_capacity(a);

    // fill exactly to capacity
    for(size_t i = 0; i < cap; ++i)
    {
        assert(array_push_back(a, &i) == 0);
    }

    assert(array_size(a) == cap);
    assert(array_capacity(a) == cap);

    int val = -777;
    // insert at beginning must trigger growth
    assert(array_insert(a, &val, 0) == 0);

    assert(array_size(a) == cap + 1);
    assert(array_capacity(a) > cap);

    int got = 0;
    assert(array_get(a, 0, &got) == 0);
    assert(got == -777);

    // check last element still correct
    assert(array_get(a, cap, &got) == 0);
    assert(got == (int)(cap - 1));

    array_delete(&a);
}

static void test_array_insert_triggers_realloc(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    size_t init_cap = array_capacity(a);

    // fill to capacity
    for(size_t i = 0; i < init_cap; ++i)
    {
        assert(array_push_back(a, &i) == 0);
    }

    assert(array_size(a) == init_cap);
    assert(array_capacity(a) == init_cap);

    int val = 999;
    assert(array_insert(a, &val, init_cap) == 0);

    assert(array_size(a) == init_cap + 1);
    assert(array_capacity(a) > init_cap);

    array_delete(&a);
}

static void test_array_insert_no_change_enough_space(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    size_t init_cap = array_capacity(a);

    // fill leaving 3 free slots
    for(size_t i = 0; i < init_cap - 3; ++i)
    {
        assert(array_push_back(a, &i) == 0);
    }

    assert(array_capacity(a) == init_cap);

    int val = -1;
    assert(array_insert(a, &val, 0) == 0);             // front
    assert(array_insert(a, &val, array_size(a)) == 0); // back
    assert(array_insert(a, &val, 5) == 0);             // middle

    assert(array_size(a) == init_cap - 3 + 3);
    assert(array_capacity(a) == init_cap); // no growth

    array_delete(&a);
}

static void test_array_insert_back(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    // empty array initially
    assert_array_invariants(a);

    size_t initial_capacity = array_capacity(a);

    // insert 20 elements at the back using insert at current size
    for(int i = 0; i < 20; ++i)
    {
        assert(array_insert(a, &i, array_size(a)) == 0);
        assert(array_size(a) == (size_t)i + 1);
    }

    // verify preserved order and values after multiple reallocations
    for(int i = 0; i < 20; ++i)
    {
        int value = -1;
        assert(array_get(a, (size_t)i, &value) == 0);
        assert(value == i);
    }

    // capacity must have grown beyond initial value
    assert(array_capacity(a) > initial_capacity);

    array_delete(&a);
}

static void test_array_insert_front(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    // empty array initially
    assert_array_invariants(a);

    size_t initial_capacity = array_capacity(a);

    // values to insert at front (order in array will be reversed)
    const int values[] = {5, 4, 3, 2, 1};
    const int n = sizeof(values) / sizeof(values[0]);

    // repeatedly insert at position 0
    for(int i = 0; i < n; ++i)
    {
        assert(array_insert(a, &values[i], 0) == 0);
        assert(array_size(a) == (size_t)(i + 1));
    }

    // verify reversed order after all front insertions
    for(int i = 0; i < n; ++i)
    {
        int out = 0;
        assert(array_get(a, (size_t)i, &out) == 0);
        assert(out == values[n - i - 1]);
    }

    // capacity should not shrink (at least initial)
    assert(array_capacity(a) >= initial_capacity);

    array_delete(&a);
}

void run_array_insert_tests(void)
{
    test_array_insert_empty_at_zero();
    test_array_insert_at_end_on_empty();
    test_array_insert_single_element_middle();
    test_array_insert_front_on_nonempty();
    test_array_insert_back_equivalent_to_push();
    test_array_insert_middle_single();
    test_array_insert_at_last_position();
    test_array_insert_when_full_capacity();
    test_array_insert_triggers_realloc();
    test_array_insert_no_change_enough_space();
    test_array_insert_back();
    test_array_insert_front();
}
