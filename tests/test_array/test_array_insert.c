#include "../include/array.h"

#include <assert.h>
#include <errno.h>

#ifdef __GNUC__
#define MAYBE_UNUSED __attribute__((unused))
#else
#define MAYBE_UNUSED
#endif

static void
assert_array_invariants(const Array *array MAYBE_UNUSED)
{
    assert(array != NULL);
    assert(array_capacity(array) >= array_size(array));
}

static void
test_array_insert_into_empty(void)
{
    Array *array = array_init(sizeof(int));
    assert(array);
    assert_array_invariants(array);

    const size_t initial_size = array_size(array);

    int value = 1;

    assert(array_insert(array, &value, 0) == 0);

    assert(array_size(array) == 1);
    assert(array_size(array) == initial_size + 1);

    int output;
    assert(array_get(array, 0, &output) == 0);
    assert(output == value);

    assert_array_invariants(array);

    array_delete(&array);
}

static void
test_array_insert_at_front(void)
{
    /// arrange

    Array *array = array_init(sizeof(int));
    assert(array);

    assert_array_invariants(array);

    const int values[] = {1, 2, 3, 4, 5};
    const size_t n = sizeof(values) / sizeof(values[0]);

    /// act

    for(size_t i = 0; i < n; ++i)
    {
        assert(array_insert(array, &values[i], 0) == 0);

        // size must increase
        assert(array_size(array) == i + 1);

        // invariants must hold
        assert_array_invariants(array);
    }

    /// assert

    for(size_t i = 0; i < n; ++i)
    {
        int output;
        assert(array_get(array, i, &output) == 0);

        // compare with original reversed sequence
        assert(output == values[n - i - 1]);
    }

    /// act

    int value = 1;
    const size_t initial_size = array_size(array);
    assert(array_insert(array, &value, 0) == 0);

    /// assert

    assert(array_size(array) == initial_size + 1);

    int output;
    assert(array_get(array, 0, &output) == 0);
    assert(output == value);

    for(size_t i = 1; i < array_size(array); ++i)
    {
        int current MAYBE_UNUSED = 0;
        int previous MAYBE_UNUSED = 0;

        assert(array_get(array, i, &current) == 0);
        assert(array_get(array, i - 1, &previous) == 0);

        // compare with original reversed sequence
        assert(current == values[n - i]);
    }

    assert_array_invariants(array);

    array_delete(&array);
}

static void
test_array_insert_in_middle(void)
{
    /// arrange

    Array *array = array_init(sizeof(int));
    assert(array);
    assert_array_invariants(array);

    const int initial[] = {1, 2, 3, 4, 5};
    const size_t n = sizeof(initial) / sizeof(initial[0]);

    for(size_t i = 0; i < n; ++i)
    {
        assert(array_insert(array, &initial[i], i) == 0);

        assert_array_invariants(array);
    }

    /// act

    const size_t old_size = array_size(array);

    int value = 100;
    assert(array_insert(array, &value, 3) == 0); // insert in middle

    /// assert

    assert(array_size(array) == old_size + 1);

    const int expect[] = {1, 2, 3, 100, 4, 5};

    // sanity check
    for(size_t i = 0; i < n + 1; ++i)
    {
        int output;
        assert(array_get(array, i, &output) == 0);
        assert(output == expect[i]);
    }

    assert_array_invariants(array);

    array_delete(&array);
}

static void
test_array_insert_at_back(void)
{
    /// arrange

    Array *array = array_init(sizeof(int));
    assert(array);

    const int initial[] = {1, 2, 3, 4, 5};
    const size_t n = sizeof(initial) / sizeof(initial[0]);

    for(size_t i = 0; i < n; ++i)
    {
        assert(array_insert(array, &initial[i], i) == 0);
    }

    /// act

    const size_t old_size = array_size(array);

    int value = 100;
    assert(array_insert(array, &value, array_size(array)) == 0);

    /// assert

    assert(array_size(array) == old_size + 1);

    const int expect[] = {1, 2, 3, 4, 5, 100};

    // sanity check
    for(size_t i = 0; i < n + 1; ++i)
    {
        int output = 0;
        assert(array_get(array, i, &output) == 0);
        assert(output == expect[i]);
    }

    assert_array_invariants(array);

    array_delete(&array);
}

static void
test_array_insert_trigger_capacity_growth(void)
{
    /// arrange

    Array *a = array_init(sizeof(int));
    assert(a);
    assert_array_invariants(a);

    size_t initial_size = array_size(a);
    size_t initial_capacity = array_capacity(a);

    for(size_t i = 0; i < initial_capacity; ++i)
    {
        int value = (int)i;
        assert(array_insert(a, &value, initial_size) == 0);
    }

    assert(array_size(a) >= initial_size);
    assert(array_capacity(a) >= initial_capacity);

    /// act

    int value = 1;
    assert(array_insert(a, &value, 0) == 0);

    /// assert

    assert(array_size(a) >= initial_size + 1);
    assert(array_capacity(a) > initial_capacity);

    int output;
    assert(array_get(a, 0, &output) == 0);
    assert(output == value);

    int last_element;
    assert(array_get(a, 1, &last_element) == 0);
    assert(last_element == (int)(initial_capacity - 1));

    assert_array_invariants(a);
    array_delete(&a);
}

static void
test_array_insert_invalid_args(void)
{
    /// arrange
    Array *array = array_init(sizeof(int));
    assert(array);

    int value = 10;

    /// act + assert
    assert(array_insert(NULL, &value, 0) == EINVAL);
    assert(array_insert(array, NULL, 0) == EINVAL);

    array_delete(&array);
}

void
run_array_insert_tests(void)
{
    test_array_insert_into_empty();
    test_array_insert_at_front();
    test_array_insert_in_middle();
    test_array_insert_at_back();
    test_array_insert_trigger_capacity_growth();
    test_array_insert_invalid_args();
}
