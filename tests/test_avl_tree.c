#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "avl_tree.h"

static void test_avl_tree_init(void)
{
    printf("\n[ RUN ] test_avl_tree_init\n");

    AVLTree *tree = avl_tree_init();

    assert(tree != NULL);
    assert(tree->root == NULL);

    avl_tree_free(tree->root);
    free(tree);

    printf("[ PASS ] test_avl_tree_init\n");
}

void run_avl_tree_tests(void)
{
    test_avl_tree_init();
}
