#include "array.h"

#include <assert.h>
#include <errno.h>

static void
test_add_safe_smoke(void)
{
    size_t output = 1;

    int error = add_safe(5, 5, &output);
    assert(error == 0);
    assert(output == 10);
}

static void
test_add_safe_null(void)
{
    size_t output = 1; // this should be rewritten

    int error = add_safe(1, 1, NULL);
    assert(error != 0);
    assert(output == 1); // must not change
}

static void
test_add_safe_wraparound(void)
{
    size_t output = 1;
    int error;

    error = add_safe(0, 0, &output);
    assert(error == 0);
    assert(output == 0);

    error = add_safe(0, SIZE_MAX, &output);
    assert(error == 0);
    assert(output == SIZE_MAX);

    error = add_safe(SIZE_MAX, 0, &output);
    assert(error == 0);
    assert(output == SIZE_MAX);

    error = add_safe(SIZE_MAX, SIZE_MAX, &output);
    assert(error != 0);
    assert(output == SIZE_MAX);
}

static void
test_add_safe_no_overflow(void)
{
    size_t output = 1;
    int error;

    error = add_safe(SIZE_MAX, 1, &output);
    assert(error != 0);
    assert(output == 1);

    error = add_safe(1, SIZE_MAX, &output);
    assert(error != 0);
    assert(output == 1);

    error = add_safe(SIZE_MAX - 1, 1, &output);
    assert(error == 0);
    assert(output == SIZE_MAX);

    error = add_safe(1, SIZE_MAX - 1, &output);
    assert(error == 0);
    assert(output == SIZE_MAX);
}

static void
test_add_safe_self_alias(void)
{
    size_t out1 = 1;
    size_t out2 = 1;

    int er;

    er = add_safe(out1, out2, &out1);
    assert(er == 0);
    assert(out1 == 2);

    er = add_safe(out1, out2, &out2);
    assert(er == 0);
    assert(out2 == 3);
}

void
run_overflow_tests(void)
{
    test_add_safe_smoke();
    test_add_safe_null();
    test_add_safe_wraparound();
    test_add_safe_no_overflow();
    test_add_safe_self_alias();
}
