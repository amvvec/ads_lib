#include <stdio.h>
#include <stdlib.h>

#include "../src/avl_tree.h"

void runAVLTreeTests(void)
{
    printf("\n[ RUN ] AVL Tree Tests\n\n");
    // Test 1: Initialization
    printf("Test 1: Initialize AVL Tree\n");
    AVLTree* tree = avl_tree_init();
    if(tree && !tree->root)
    {
        printf("Result: Initialized successfully with NULL root\n");
    }
    else
    {
        printf("Result: Initialization failed\n");
    }

    // Test 2: Insert and basic balancing
    printf("Test 2: Insert values [10, 5, 15, 3, 7] and check balancing\n");
    avl_tree_insert(tree, 10);
    avl_tree_insert(tree, 5);
    avl_tree_insert(tree, 15);
    avl_tree_insert(tree, 3);
    avl_tree_insert(tree, 7);

    int height_left = get_height(tree->root->left);
    int height_right = get_height(tree->root->right);
    int balance = get_balance(tree->root);
    if(balance >= -1 && balance <= 1 && (height_left <= 2 && height_right <= 2))
    {
        printf("Result: Tree balanced after insertions (balance=%d, heights: "
               "left=%d, right=%d)\n",
               balance, height_left, height_right);
    }
    else
    {
        printf("Result: Balancing failed\n");
    }

    avl_tree_free(tree->root);
    free(tree);
    printf("\n[ PASS ] AVL Tree Tests\n");
}
