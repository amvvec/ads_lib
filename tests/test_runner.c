#include <stdio.h>

#include "test_array_new_delete.c"
#include "test_avl_tree.c"
#include "test_runner.h"

void test_runner(void)
{
    run_array_tests();
    run_avl_tree_tests();
}
