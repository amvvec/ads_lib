#include "../include/array.h"

#include <assert.h>
#include <errno.h>

static int
get_int(const Array *a, size_t index)
{
    int value;
    assert(array_get(a, index, &value) == 0);
    return value;
}

#ifdef __GNUC__
#define MAYBE_UNUSED __attribute__((unused))
#else
#define MAYBE_UNUSED
#endif

static void
assert_array_invariants(const Array *a MAYBE_UNUSED)
{
    assert(a != NULL);
    assert(array_capacity(a) >= array_size(a));
}

static void
test_array_insert_empty(void)
{
    Array *a = array_init(sizeof(int));
    assert(a);

    int v = 1;
    assert(array_insert(a, &v, 0) == 0);

    assert(array_size(a) == 1);
    assert(get_int(a, 0) == 1);

    assert_array_invariants(a);
    array_delete(&a);
}

static void
test_array_insert_at_front(void)
{
    Array *a = array_init(sizeof(int));
    assert(a);

    assert_array_invariants(a);

    const int values[] = {1, 2, 3, 4, 5};
    const size_t n = sizeof(values) / sizeof(values[0]);

    for(size_t i = 0; i < n; ++i)
    {
        assert(array_insert(a, &values[i], 0) == 0);

        // size must increase
        assert(array_size(a) == i + 1);

        // invariants must hold
        assert_array_invariants(a);
    }

    for(size_t i = 0; i < n; ++i)
    {
        // compare with original reversed sequence
        assert(get_int(a, i) == values[n - i - 1]);
    }

    int value = 1;
    const size_t initial_size = array_size(a);
    assert(array_insert(a, &value, 0) == 0);

    assert(array_size(a) == initial_size + 1);

    assert(get_int(a, 0) == value);

    for(size_t i = 1; i < array_size(a); ++i)
    {
        // compare with original reversed sequence
        assert(get_int(a, i) == values[n - i]);
    }

    assert_array_invariants(a);

    array_delete(&a);
}

static void
test_array_insert_in_middle(void)
{
    Array *a = array_init(sizeof(int));
    assert(a);
    assert_array_invariants(a);

    const int initial[] = {1, 2, 3, 4, 5};
    const size_t n = sizeof(initial) / sizeof(initial[0]);

    for(size_t i = 0; i < n; ++i)
    {
        assert(array_insert(a, &initial[i], i) == 0);

        assert_array_invariants(a);
    }

    const size_t old_size = array_size(a);

    int value = 100;
    assert(array_insert(a, &value, 3) == 0); // insert in middle

    assert(array_size(a) == old_size + 1);

    const int expect[] = {1, 2, 3, 100, 4, 5};

    // sanity check
    for(size_t i = 0; i < n + 1; ++i)
    {
        assert(get_int(a, i) == expect[i]);
    }

    assert_array_invariants(a);

    array_delete(&a);
}

static void
test_array_insert_at_back(void)
{
    Array *a = array_init(sizeof(int));
    assert(a);

    const int initial[] = {1, 2, 3, 4, 5};
    const size_t n = sizeof(initial) / sizeof(initial[0]);

    for(size_t i = 0; i < n; ++i)
    {
        assert(array_insert(a, &initial[i], i) == 0);
    }

    const size_t old_size = array_size(a);

    int value = 100;
    assert(array_insert(a, &value, array_size(a)) == 0);

    assert(array_size(a) == old_size + 1);

    const int expect[] = {1, 2, 3, 4, 5, 100};

    // sanity check
    for(size_t i = 0; i < n + 1; ++i)
    {
        assert(get_int(a, i) == expect[i]);
    }

    assert_array_invariants(a);

    array_delete(&a);
}

static void
test_array_insert_trigger_capacity_growth(void)
{
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

    int value = 1;
    assert(array_insert(a, &value, 0) == 0);

    assert(array_size(a) >= initial_size + 1);
    assert(array_capacity(a) > initial_capacity);

    assert(get_int(a, 0) == value);

    assert(get_int(a, 1) == (int)(initial_capacity - 1));

    assert_array_invariants(a);
    array_delete(&a);
}

static void
test_array_insert_invalid_args(void)
{
    Array *a = array_init(sizeof(int));
    assert(a);
    assert_array_invariants(a);

    int value = 1;

    assert(array_insert(NULL, &value, 0) == EINVAL);
    assert(array_insert(a, NULL, 0) == EINVAL);

    assert_array_invariants(a);
    array_delete(&a);
}

void
run_array_insert_tests(void)
{
    test_array_insert_empty();
    test_array_insert_at_front();
    test_array_insert_in_middle();
    test_array_insert_at_back();
    test_array_insert_trigger_capacity_growth();
    test_array_insert_invalid_args();
}
