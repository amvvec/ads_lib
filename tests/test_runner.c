#include <stdio.h>

#include "test_runner.h"

#include "test_array/test_array.c"
#include "test_array/test_array_erase.c"
#include "test_array/test_array_init.c"
#include "test_array/test_array_insert.c"

void test_runner(void)
{
    printf("Running tests...\n");

    run_array_erase_tests();
    run_array_init_tests();
    run_array_insert_tests();
    run_array_smoke_tests();

    printf("All tests passed\n");
}
