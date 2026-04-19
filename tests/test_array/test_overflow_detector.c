#include "array.h"

#include <assert.h>
#include <errno.h>

static void
test_safe_add_smoke(void)
{
    size_t output = 1;

    int error = safe_add(5, 5, &output);
    assert(error == 0);
    assert(output == 10);
}

static void
test_safe_add_null(void)
{
    size_t output = 1; // this should be rewritten

    int error = safe_add(1, 1, NULL);
    assert(error != 0);
    assert(output == 1); // must not change
}

static void
test_safe_add_wraparound(void)
{
    size_t output = 1;
    int error;

    error = safe_add(0, 0, &output);
    assert(error == 0);
    assert(output == 0);

    error = safe_add(0, SIZE_MAX, &output);
    assert(error == 0);
    assert(output == SIZE_MAX);

    error = safe_add(SIZE_MAX, 0, &output);
    assert(error == 0);
    assert(output == SIZE_MAX);

    error = safe_add(SIZE_MAX, SIZE_MAX, &output);
    assert(error != 0);
    assert(output == SIZE_MAX);
}

static void
test_safe_add_no_overflow(void)
{
    size_t output = 1;
    int error;

    error = safe_add(SIZE_MAX, 1, &output);
    assert(error != 0);
    assert(output == 1);

    error = safe_add(1, SIZE_MAX, &output);
    assert(error != 0);
    assert(output == 1);

    error = safe_add(SIZE_MAX - 1, 1, &output);
    assert(error == 0);
    assert(output == SIZE_MAX);

    error = safe_add(1, SIZE_MAX - 1, &output);
    assert(error == 0);
    assert(output == SIZE_MAX);
}

void
run_overflow_tests(void)
{
    test_safe_add_smoke();
    test_safe_add_null();
    test_safe_add_wraparound();
    test_safe_add_no_overflow();
}
