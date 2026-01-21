#include <assert.h>

#include "../src/array.h"

static void test_array_init_overflow(void)
{
    size_t element_size_overflow = (SIZE_MAX / 8) + 1;
    Array *a = array_init(element_size_overflow);
    assert(a == NULL);
    array_delete(&a);
}

void run_array_init_tests(void)
{
    test_array_init_overflow();
}
