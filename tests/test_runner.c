#include <stdio.h>

#include "test_array.c"
#include "test_avl_tree.c"
#include "test_runner.h"

void test_runner(void)
{
    printf("Running tests...\n");
    
    run_array_tests();
    // run_avl_tree_tests();
    
    printf("All tests passed\n");
}
