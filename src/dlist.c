#include "dlist.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/** @struct DNode
 *  @brief Represents a node in a doubly linked list.
 *  @var DNode::data Integer data stored in the node.
 *  @var DNode::prev Pointer to the previous node in the list.
 *  @var DNode::next Pointer to the next node in the list.
 */
struct DNode
{
    int data;           /**< Integer data stored in the node. */
    struct DNode *prev; /**< Pointer to the previous node in the list. */
    struct DNode *next; /**< Pointer to the next node in the list. */
};

/** @brief Creates a new node with the given data.
 *  @param data The integer value to store in the node.
 *  @return Pointer to the new node, or NULL if memory allocation fails or data
 * is out of range.
 *  @note Checks for integer overflow and memory allocation errors. O(1) time
 * complexity.
 */
DNode *createDNode(int data)
{
    if(data > INT_MAX || data < INT_MIN)
    {
        fprintf(stderr, "Data value out of integer range\n");
        return NULL;
    }
    if(sizeof(DNode) > SIZE_MAX / 2)
    {
        fprintf(stderr, "Memory allocation size too large\n");
        return NULL;
    }

    DNode *node = (DNode *)calloc(1, sizeof(DNode));
    if(!node)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

/** @brief Inserts a new node with the given data at the front of the list.
 *  @param head Double pointer to the list's head.
 *  @param data The integer value to insert.
 *  @note Handles null head pointer and failed node creation. O(1) time
 * complexity.
 */

void insertDFront(DNode **head, int data)
{
    if(!head)
    {
        fprintf(stderr, "Invalid head pointer\n");
        return;
    }
    if(data > INT_MAX / 2 || data < INT_MIN / 2)
    {
        fprintf(stderr, "Data out of range, potential injection\n");
        return;
    }

    DNode *newNode = createDNode(data);
    if(!newNode)
    {
        return;
    }
    newNode->next = *head;
    if(*head)
    {
        (*head)->prev = newNode;
    }
    *head = newNode;
}

/** @brief Inserts a new node with the given data at the end of the list.
 *  @param head Double pointer to the list's head.
 *  @param data The integer value to insert.
 *  @note Handles null head pointer, empty list, and failed node creation. O(n)
 * time complexity.
 */
void insertDEnd(DNode **head, int data)
{
    if(!head)
    {
        fprintf(stderr, "Invalid head pointer\n");
        return;
    }
    DNode *newNode = createDNode(data);
    if(!head)
    {
        fprintf(stderr, "Invalid head pointer\n");
        return;
    }
    if(!*head)
    {
        *head = newNode;
        return;
    }

    DNode *tmp = *head;
    while(tmp->next)
    {
        tmp = tmp->next;
    }
    tmp->next = newNode;
    newNode->prev = tmp;
}

/** @brief Deletes the first node with the given data from the list.
 *  @param head Double pointer to the list's head.
 *  @param data The integer value to delete.
 *  @note Handles empty list, null head pointer, and non-existent data. O(n)
 * time complexity.
 */
void deleteDNode(DNode **head, int data)
{
    if(!head || !*head)
    {
        fprintf(stderr, "Empty list or invalid head pointer\n");
        return;
    }

    DNode *tmp = *head;
    if(tmp->data == data)
    {
        *head = tmp->next;
        if(*head)
        {
            (*head)->prev = NULL;
        }
        free(tmp);
        tmp = NULL;
        return;
    }

    while(tmp && tmp->data != data)
    {
        tmp = tmp->next;
    }

    if(!tmp)
    {
        fprintf(stderr, "Data %d not found\n", data);
        return;
    }
    if(tmp->prev)
    {
        tmp->prev->next = tmp->next;
    }
    if(tmp->next)
    {
        tmp->next->prev = tmp->prev;
    }
    free(tmp);
    tmp = NULL;
}

/** @brief Prints all nodes in the list.
 *  @param head Pointer to the list's head.
 *  @note Prints "Empty list" if head is NULL. O(n) time complexity.
 */
void printDList(DNode *head)
{
    if(!head)
    {
        printf("Empty list\n");
        return;
    }

    DNode *tmp = head;
    while(tmp)
    {
        printf("%d ", tmp->data);
        tmp = tmp->next;
    }
    printf("\n");
}

/** @brief Frees all nodes in the list.
 *  @param head Double pointer to the list's head.
 *  @note Handles null head pointer. Sets head to NULL after freeing. O(n) time
 * complexity.
 */
void freeDList(DNode **head)
{
    if(!head)
    {
        fprintf(stderr, "Invalid head pointer\n");
        return;
    }

    DNode *tmp;
    while(*head)
    {
        tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }

    *head = NULL;
}
