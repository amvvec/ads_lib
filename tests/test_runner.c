#include <stdio.h>

#include "test_array/test_array.c"
#include "test_array/test_array_init.c"
#include "test_array/test_array_insert.c"
#include "test_avl_tree.c"
#include "test_runner.h"

void test_runner(void)
{
    printf("Running tests...\n");

    run_array_tests();
    run_array_init_tests();
    run_array_insert_tests();
    run_avl_tree_tests();

    printf("All tests passed\n");
}
