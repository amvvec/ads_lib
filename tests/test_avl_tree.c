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

static void test_avl_tree_insert_and_balance(void)
{
    printf("\n[ RUN ] test_avl_tree_insert_and_balance\n");

    AVLTree * tree = avl_tree_init();

    assert(tree != NULL);

    avl_tree_insert(tree, 10);
    avl_tree_insert(tree, 5);
    avl_tree_insert(tree, 15);
    avl_tree_insert(tree, 3);
    avl_tree_insert(tree, 7);

    AVLNode * root = tree->root;

    assert(root != NULL);
    assert(root->data == 10);
    assert(get_balance(root) >= -1 && get_balance(root) <= 1);

    assert(get_height(root->left)  == 2);
    assert(get_height(root->right) == 1);

    avl_tree_free(tree->root);
    free(tree);
    
    printf("[ OK ] test_avl_tree_insert_and_balance\n");
}

void run_avl_tree_tests(void)
{
    test_avl_tree_init();
    test_avl_tree_insert_and_balance();
}
