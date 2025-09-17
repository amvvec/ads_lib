#include "avl_tree.h"

#include <stdio.h>
#include <stdlib.h>

int max(int a, int b)
{
    return a > b ? a : b;
}

/** @brief Gets the height of a node.
 *  @param node Pointer to the AVLNode.
 *  @return The height of the node, or 0 if node is NULL. Time complexity is
 * O(1).
 */
int get_height(AVLNode* node)
{
    return node ? node->height : 0;
}

/** @brief Calculates the balance factor of a node.
 *  @param node Pointer to the AVLNode.
 *  @return The balance factor (height of left - height of right), or 0 if node
 * is NULL. Time complexity is O(1).
 */
int get_balance(AVLNode* node)
{
    return node ? get_height(node->left) - get_height(node->right) : 0;
}

/** @brief Creates a new AVL node.
 *  @param data The integer data to store in the node.
 *  @return Pointer to the new AVLNode, or NULL if memory allocation fails.
 *  @note Initializes height to 1, left and right to NULL. Time complexity is
 * O(1).
 *  @warning Returns NULL and prints an error if memory allocation fails.
 */
AVLNode* create_node(int data)
{
    AVLNode* node = (AVLNode*)calloc(1, sizeof(AVLNode));

    if(!node)
    {
        fprintf(stderr, "Memory allocation failed for avl node\n");
        return NULL;
    }

    node->data = data;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;

    return node;
}

/** @brief Performs a left rotation on the given node.
 *  @param x Pointer to the node to rotate.
 *  @return Pointer to the new root of the rotated subtree, or x if rotation is
 * invalid.
 *  @note Updates heights after rotation. Time complexity is O(1).
 *  @warning Returns x and prints an error if x or x->right is NULL.
 */
AVLNode* left_rotate(AVLNode* x)
{
    if(!x || !x->right)
    {
        fprintf(stderr, "Invalid rotation: NULL node or right child\n");
        return x;
    }

    AVLNode* y = x->right;
    AVLNode* tmp = y->left;

    y->left = x;
    x->right = tmp;

    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;

    return y;
}

/** @brief Performs a right rotation on the given node.
 *  @param y Pointer to the node to rotate.
 *  @return Pointer to the new root of the rotated subtree, or y if rotation is
 * invalid.
 *  @note Updates heights after rotation. Time complexity is O(1).
 *  @warning Returns y and prints an error if y or y->left is NULL.
 */
AVLNode* right_rotate(AVLNode* y)
{
    if(!y || !y->left)
    {
        fprintf(stderr, "Invalid rotation: NULL node or left child\n");
        return y;
    }

    AVLNode* x = y->left;
    AVLNode* tmp = x->right;

    x->right = y;
    y->left = tmp;

    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    x->height = max(get_height(x->left), get_height(x->right)) + 1;

    return x;
}

/** @brief Inserts a new value into the AVL tree.
 *  @param node Pointer to the current node.
 *  @param data The integer value to insert.
 *  @return Pointer to the root of the modified subtree.
 *  @note Performs rotations to maintain AVL balance. Average and worst time
 * complexity is O(log n).
 *  @warning Returns node and prints an error if rotation conditions are
 * invalid.
 */
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
        if(!node->left)
        {
            fprintf(stderr, "Invalid LR rotation: NULL left child\n");
            return node;
        }

        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    // RL case
    if(balance < -1 && data < node->right->data)
    {
        if(!node->right)
        {
            fprintf(stderr, "Invalid RL rotation: NULL right child\n");
            return node;
        }

        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

/** @brief Initializes a new AVL tree.
 *  @return Pointer to the new AVLTree, or NULL if memory allocation fails.
 *  @note Sets the root to NULL. Time complexity is O(1).
 *  @warning Returns NULL and prints an error if memory allocation fails.
 */
AVLTree* avl_tree_init()
{
    AVLTree* tree = (AVLTree*)malloc(sizeof(AVLTree));

    if(!tree)
    {
        fprintf(stderr, "Memory allocation failed for AVL tree\n");
        return NULL;
    }

    tree->root = NULL;

    return tree;
}

/** @brief Inserts a new value into the AVL tree.
 *  @param tree Pointer to the AVLTree.
 *  @param data The integer value to insert.
 *  @note Maintains AVL tree balance using rotations. Average and worst time
 * complexity is O(log n).
 *  @warning Does nothing if tree is NULL, with an error message to stderr.
 * Ignores duplicates.
 */
void avl_tree_insert(AVLTree* tree, int data)
{
    if(!tree)
    {
        fprintf(stderr, "Invalid tree: NULL\n");
        return;
    }

    tree->root = insert(tree->root, data);
}

/** @brief Frees the memory allocated for the AVL tree.
 *  @param node Pointer to the root node to free (recursive).
 *  @note Recursively frees all nodes. Time complexity is O(n).
 */
void avl_tree_free(AVLNode* node)
{
    if(node)
    {
        avl_tree_free(node->left);
        avl_tree_free(node->right);

        free(node);
    }
}
