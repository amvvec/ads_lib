#include "slist.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/** @struct SNode
 *  @brief Represents a node in a singly linked list.
 *  @var SNode::data Integer data stored in the node.
 *  @var SNode::next Pointer to the next node in the list.
 */
struct SNode
{
    int data;
    struct SNode * next;
};

/** @brief Creates a new node with the given data.
 *  @param data The integer value to store in the node.
 *  @return Pointer to the new node, or NULL if data is out of range or memory
 * allocation fails.
 *  @note Checks for integer overflow and memory allocation errors.
 */
SNode * createSNode(int data)
{
    if(data > INT_MAX || data < INT_MIN)
    {
        fprintf(stderr, "Data value out of integer range\n");
        return NULL;
    }
    if(sizeof(SNode) > SIZE_MAX / 2)
    {
        fprintf(stderr, "Memory allocation size too large\n");
        return NULL;
    }
    SNode * node = (SNode *)calloc(1, sizeof(SNode));
    if(!node)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->data = data;
    node->next = NULL;
    return node;
}

/** @brief Inserts a new node with the given data at the front of the list.
 *  @param head Double pointer to the list's head.
 *  @param data The integer value to insert.
 *  @return None.
 *  @note Handles null head pointer and failed node creation. O(1) time
 * complexity.
 */
void insertSFront(SNode ** head, int data)
{
    if(!head)
    {
        fprintf(stderr, "Invalid head pointer\n");
        return;
    }
    if(data > INT_MAX / 2 || data < INT_MIN / 2)
    {
        fprintf(stderr, "Data out of safe range, potential injection\n");
        return;
    }
    SNode * newNode = createSNode(data);
    if(!newNode)
    {
        return;
    }
    newNode->next = *head;
    *head = newNode;
}

/** @brief Inserts a new node with the given data at the end of the list.
 *  @param head Double pointer to the list's head.
 *  @param data The integer value to insert.
 *  @return None.
 *  @note Handles null head pointer, empty list, and failed node creation. O(n)
 * time complexity.
 */
void insertSEnd(SNode ** head, int data)
{
    if(!head)
    {
        fprintf(stderr, "Invalid head pointer\n");
        return;
    }
    SNode * newNode = createSNode(data);
    if(!newNode)
    {
        return;
    }
    if(!*head)
    {
        *head = newNode;
        return;
    }
    SNode * tmp = *head;
    while(tmp->next)
    {
        tmp = tmp->next;
    }
    tmp->next = newNode;
}

/** @brief Deletes the first node with the given data from the list.
 *  @param head Double pointer to the list's head.
 *  @param data The integer value to delete.
 *  @return None.
 *  @note Handles empty list, null head pointer, and non-existent data. O(n)
 * time complexity.
 */
void deleteSNode(SNode ** head, int data)
{
    if(!head || !*head)
    {
        fprintf(stderr, "Empty list or invalid head pointer\n");
        return;
    }
    SNode *tmp = *head, *prev = NULL;
    if(tmp && tmp->data == data)
    {
        *head = tmp->next;
        free(tmp);
        tmp = NULL; // Use-after-free safety
        return;
    }
    while(tmp && tmp->data != data)
    {
        prev = tmp;
        tmp = tmp->next;
    }
    if(!tmp)
    {
        fprintf(stderr, "Data %d not found\n", data);
        return;
    }
    prev->next = tmp->next;
    free(tmp);
}

/** @brief Prints all nodes in the list.
 *  @param head Pointer to the list's head.
 *  @return None.
 *  @note Prints "Empty list" if head is NULL. O(n) time complexity.
 */
void printSList(SNode * head)
{
    if(!head)
    {
        printf("Empty list\n");
        return;
    }
    SNode * tmp = head;
    while(tmp)
    {
        printf("%d ", tmp->data);
        tmp = tmp->next;
    }
    printf("\n");
}

/** @brief Frees all nodes in the list.
 *  @param head Double pointer to the list's head.
 *  @return None.
 *  @note Handles null head pointer. Sets head to NULL after freeing. O(n) time
 * complexity.
 */
void freeSList(SNode ** head)
{
    if(!head)
    {
        fprintf(stderr, "Invalid head pointer\n");
        return;
    }
    SNode * tmp;
    while(*head)
    {
        tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
    *head = NULL; // Use-after-free safety
}
