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
test_array_create_invalid_element_size(void)
{
    Array *a = NULL;
    assert(array_create(&a, 0) == EINVAL);
    assert(a == NULL);
}

void
test_array_destroy_null(void)
{
    array_destroy(NULL);
}

void
test_array_destroy_null_object(void)
{
    Array *a = NULL;
    array_destroy(&a);
    assert(a == NULL);
}

void
test_array_destroy_nullifies_pointer(void)
{
    Array *a = NULL;

    assert(array_create(&a, sizeof(int)) == 0);

    array_destroy(&a);
    assert(a == NULL);
}

void
run_array_create_destroy_tests(void)
{
    test_array_create_valid();
    test_array_create_null_object();
    test_array_create_invalid_element_size();
    test_array_destroy_null();
    test_array_destroy_null_object();
    test_array_destroy_nullifies_pointer();
}
