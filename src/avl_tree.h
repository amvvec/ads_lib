#ifndef AVL_TREE_H
#define AVL_TREE_H

/** @struct AVLNode
 *  @brief A node in the AVL tree, representing a single element with balancing
 * information.
 *  @var AVLNode::data The integer data stored in the node.
 *  @var AVLNode::height The height of the subtree rooted at this node.
 *  @var AVLNode::left Pointer to the left child node.
 *  @var AVLNode::right Pointer to the right child node.
 */
typedef struct AVLNode
{
    int data;             /**< The integer data stored in the node. */
    int height;           /**< The height of the subtree rooted at this node. */
    struct AVLNode* left; /**< Pointer to the left child node. */
    struct AVLNode* right; /**< Pointer to the right child node. */
} AVLNode;

/** @struct AVLTree
 *  @brief The AVL tree structure, containing the root node.
 *  @var AVLTree::root Pointer to the root node of the AVL tree.
 */
typedef struct
{
    AVLNode* root;
} AVLTree;

AVLNode* insert(AVLNode* node, int data);
AVLTree* avl_tree_init();
void avl_tree_insert(AVLTree* tree, int data);
void avl_tree_free();
int get_height(AVLNode* node);
int get_balance(AVLNode* node);

#endif // !AVL_TREE_H
