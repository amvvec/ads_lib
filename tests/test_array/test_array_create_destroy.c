#include "../include/array.h"

#include <assert.h>

void
test_array_create_valid(void)
{
    Array *a = NULL;
    assert(array_create(&a, sizeof(int)) == 0);
    assert(a);

    array_destroy(&a);
}

void
run_array_create_destroy_tests(void)
{
    test_array_create_valid();
}
