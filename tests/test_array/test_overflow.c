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

void
run_overflow_tests(void)
{
    test_safe_add_smoke();
}
