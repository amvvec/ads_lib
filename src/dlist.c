#include "dlist.h"

#include <stdio.h>
#include <stdlib.h>

struct DNode
{
    int data;
    struct DNode* prev;
    struct DNode* next;
};

DNode* createDNode(int data)
{
    if (data > INT_MAX || data < INT_MIN)
    {
        fprintf(stderr, "Data value out of integer range\n");
        return NULL;
    }
    DNode* node = (DNode*)malloc(sizeof(DNode));
    if (!node)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void insertDFront(DNode** head, int data)
{
    if (!head)
    {
        fprintf(stderr, "Invalid head pointer\n");
        return;
    }
    DNode* newNode = createDNode(data);
    if (!newNode)
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

void insertDEnd(DNode** head, int data)
{
    if (!head)
    {
        fprintf(stderr, "Invalid head pointer\n");
        return;
    }
    DNode* newNode = createDNode(data);
    if (!head)
    {
        fprintf(stderr, "Invalid head pointer\n");
        return;
    }
    if(!*head)
    {
        *head = newNode;
        return;
    }
    DNode* tmp = *head;
    while(tmp->next)
    {
        tmp = tmp->next;
    }
    tmp->next = newNode;
    newNode->prev = tmp;
}

void deleteDNode(DNode** head, int data)
{
    if (!head || !*head)
    {
        fprintf(stderr, "Empty list or invalid head pointer\n");
        return;
    }
    DNode* tmp = *head;
    if (tmp->data == data)
    {
        *head = tmp->next;
        if (*head)
        {
            (*head)->prev = NULL;
        }
        free(tmp);
        return;
    }
    while (tmp && tmp->data != data)
    {
        tmp = tmp->next;
    }
    if (!tmp)
    {
        fprintf(stderr, "Data %d not found\n", data);
        return;
    }
    if (tmp->prev)
    {
        tmp->prev->next = tmp->next;
    }
    if (tmp->next)
    {
        tmp->next->prev = tmp->prev;
    }
    free(tmp);
}

void printDList(DNode* head)
{
    if (!head)
    {
        printf("Empty list\n");
        return;
    }
    DNode* tmp = head;
    while (tmp)
    {
        printf("%d ", tmp->data);
        tmp = tmp->next;
    }
    printf("\n");
}

void freeDList(DNode** head)
{
    if (!head)
    {
        fprintf(stderr, "Invalid head pointer\n");
        return;
    }
    DNode* tmp;
    while (*head)
    {
        tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
}
