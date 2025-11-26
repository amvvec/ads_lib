#include <stdio.h>

#include "../tests/test_array/array_new_delete.c"
#include "../tests/test_array/array_push_back.c"
#include "test_runner.h"

void test_runner(void)
{
    run_array_tests_new_delete();
    // run_array_tests_push_back();
}
