#include "../include/array.h"

#include <assert.h>
#include <errno.h>

void
test_array_create_valid(void)
{
    Array *a = NULL;
    assert(array_create(&a, sizeof(int)) == 0);
    assert(a);

    array_destroy(&a);
}

void
test_array_create_null_object(void)
{
    assert(array_create(NULL, sizeof(int)) == EINVAL);
}

void
run_array_create_destroy_tests(void)
{
    test_array_create_valid();
    test_array_create_null_object();
}
