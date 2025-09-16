#ifndef AVL_TREE_H
#define AVL_TREE_H

typedef struct AVLNode
{
    int data;
    int height;
    struct AVLNode* left;
    struct AVLNode* right;
} AVLNode;

typedef struct
{
    AVLNode* root;
} AVLTree;

AVLNode* insert(AVLNode* node, int data);
AVLTree* avl_tree_init();
void avl_tree_free();
int get_height(AVLNode * node);
int get_balance(AVLNode * node);

#endif // !AVL_TREE_H
