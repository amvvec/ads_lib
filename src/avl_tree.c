#include "avl_tree.h"

#include <stdlib.h>

int max(int a, int b)
{
    return a > b ? a : b;
}

int get_height(AVLNode* node)
{
    return node ? node->height : 0;
}

int get_balance(AVLNode* node)
{
    return node ? get_height(node->left) - get_height(node->right) : 0;
}

AVLNode* create_node(int data)
{
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));

    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;

    return node;
}

AVLNode* left_rotate(AVLNode* x)
{
    AVLNode* y = x->right;
    AVLNode* tmp = y->left;

    y->left = x;
    x->right = tmp;

    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;

    return y;
}

AVLNode* right_rotate(AVLNode* y)
{
    AVLNode* x = y->left;
    AVLNode* tmp = x->right;

    x->right = y;
    y->left = tmp;

    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    x->height = max(get_height(x->left), get_height(x->right)) + 1;

    return x;
}

AVLNode* insert(AVLNode* node, int data)
{
    if(!node)
    {
        return create_node(data);
    }

    if(data < node->data)
    {
        node->left = insert(node->left, data);
    }
    else if(data > node->data)
    {
        node->right = insert(node->right, data);
    }
    else
    {
        return node;
    }

    node->height = max(get_height(node->left), get_height(node->right)) + 1;

    int balance = get_balance(node);

    // LL case
    if(balance > 1 && data < node->left->data)
    {
        return right_rotate(node);
    }

    // RR case
    if(balance < -1 && data > node->right->data)
    {
        return left_rotate(node);
    }

    // LR case
    if(balance > 1 && data > node->left->data)
    {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    // RL case
    if(balance < -1 && data < node->right->data)
    {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

AVLTree* avl_tree_init()
{
    AVLTree* tree = (AVLTree*)malloc(sizeof(AVLTree));

    tree->root = NULL;

    return tree;
}

void avl_tree_insert(AVLTree* tree, int data)
{
    if(!tree)
    {
        return;
    }

    tree->root = insert(tree->root, data);
}

void avl_tree_free(AVLNode* node)
{
    if(node)
    {
        avl_tree_free(node->left);
        avl_tree_free(node->right);

        free(node);
    }
}
