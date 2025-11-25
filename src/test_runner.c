#include "test_runner.h"
#include "../tests/test_array/test_array.c"
#include "../tests/test_avl_tree.c"
#include "../tests/test_bfs.c"
#include "../tests/test_binary_search.c"
#include "../tests/test_bubble_sort.c"
#include "../tests/test_dfs.c"
#include "../tests/test_dijkstra.c"
#include "../tests/test_dlist.c"
#include "../tests/test_graph.c"
#include "../tests/test_hash_table.c"
#include "../tests/test_insertion_sort.c"
#include "../tests/test_merge_sort.c"
#include "../tests/test_quick_sort.c"
#include "../tests/test_skip_list.c"
#include "../tests/test_slist.c"
#include "../tests/test_vector.c"
#include <stdio.h>

void test_runner(void)
{
    run_array_tests();
}
