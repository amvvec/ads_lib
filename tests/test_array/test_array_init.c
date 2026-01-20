#include <assert.h>

#include "../src/array.h"

static void test_array_init_overflow(void)
{
    // lets assume that init will overflow with SIZE_MAX
    size_t element_size_overflow = (SIZE_MAX / 8) + 1;
    Array *a = array_init(
        element_size_overflow); // ARR_INIT_CAP * elem_size -> overflow
    assert(a == NULL);          // mult_overflow
}

void run_array_init_tests(void)
{
    test_array_init_overflow();
}
