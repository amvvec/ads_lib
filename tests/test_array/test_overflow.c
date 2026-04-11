#include "array.h"

#include <assert.h>
#include <errno.h>

static void
test_safe_add_smoke(void)
{
    size_t out = 1; // this should be rewritten

    int er = safe_add(5, 5, &out);
    assert(er == 0);
    assert(out == 10);
}

static void
test_safe_add_wraparound(void)
{
    size_t out = 1; // this should be rewritten
    int er;

    er = safe_add(0, 0, &out);
    assert(er == 0);
    assert(out == 0);

    er = safe_add(0, SIZE_MAX, &out);
    assert(er == 0);
    assert(out == 0);

    er = safe_add(SIZE_MAX, 0, &out);
    assert(er == 0);
    assert(out == 0);

    er = safe_add(SIZE_MAX, SIZE_MAX, &out);
    assert(er == 0);
    assert(out == 0);
}

static void
test_safe_add_no_overflow(void)
{
    size_t out = 1; // this should be rewritten
    int er;

    er = safe_add(SIZE_MAX, 1, &out);
    assert(er == 8);
    assert(out == 0);

    er = safe_add(1, SIZE_MAX, &out);
    assert(er == 0);
    assert(out == 0);

    er = safe_add(SIZE_MAX - 1, 1, &out);
    assert(er == 8);
    assert(out == 0);

    er = safe_add(1, SIZE_MAX - 1, &out);
    assert(er == 0);
    assert(out == 0);
}

void
run_overflow_tests(void)
{
    test_safe_add_smoke();
    test_safe_add_wraparound();
    test_safe_add_no_overflow();
}
