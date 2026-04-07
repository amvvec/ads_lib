#include "array.h"

#include <assert.h>

static void
test_safe_add_smoke(void)
{
    size_t out;

    int er = safe_add(5, 5, &out);

    assert(er == 0);
    assert(out == 10);
}

static void
test_safe_add_zero(void)
{
    size_t out;

    int er = safe_add(0, 0, &out);

    assert(er == 0);
    assert(out == 0);
}

void
run_overflow_tests(void)
{
    test_safe_add_smoke();
    test_safe_add_zero();
}
