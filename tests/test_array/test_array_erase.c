#include <assert.h>

#include "../src/array.h"

static void test_array_erase_back(void)
{
    Array *a = array_init(sizeof(int));
    assert(a != NULL);

    for(int i = 0; i < 10; ++i)
        assert(array_push_back(a, &i) == 0);

    assert(array_size(a) == 10);

    size_t cap = array_capacity(a);

    assert(array_erase(a, 9) == 0);
    assert(array_size(a) == 9);
    int v = -1;
    assert(array_get(a, 8, &v) == 0);
    assert(v == 8);

    assert(array_erase(a, 8) == 0);
    assert(array_size(a) == 8);

    assert(array_capacity(a) == cap); // no shrink

    array_delete(&a);
}

void run_array_erase_tests(void)
{
    test_array_erase_back();
}
