#include <stdio.h>
#include <stdlib.h>
#include "slist.h"

struct SNode
{
    int data;
    struct SNode* next;
};

SNode* createNode(int data)
{
    if(data > INT_MAX || data < INT_MIN)
    {
        fprintf(stderr, "Data value out of integer range\n");
        return NULL;
    }

    SNode* node = (SNode*)malloc(sizeof(SNode));
    
    if (!node)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->data = data;
    node->next = NULL;
    return node;
}

void insertFront(SNode** head, int data)
{
    if(!head)
    {
        fprintf(stderr, "Invalid head pointer\n");
        return;
    }
    
    SNode* newNode = createNode(data);

    if(!newNode)
    {
        return;
    }
    
    newNode->next = *head;
    *head = newNode;
}

void insertEnd(SNode** head, int data)
{
    if(!head)
    {
        fprintf(stderr, "Invalid head pointer\n");
        return;
    }
    
    SNode* newNode = createNode(data);

    if(!newNode)
    {
        return;
    }
    
    if (!*head)
    {
        *head = newNode;
        return;
    }
    SNode* tmp = *head;
    while (tmp->next)
    {
        tmp = tmp->next;
    }
    tmp->next = newNode;
}

void deleteNode(SNode** head, int data)
{
    if(!head || !*head)
    {
        fprintf(stderr, "Empty list or invalid head pointer\n");
    }
    
    SNode* tmp = *head, *prev = NULL;
    if (tmp && tmp->data == data)
    {
        *head = tmp->next;
        free(tmp);
        return;
    }
    while (tmp && tmp->data != data)
    {
        prev = tmp;
        tmp = tmp->next;
    }
    if (!tmp)
    {
        fprintf("Data %d not found\n", data);
        return;
    }
    prev->next = tmp->next;
    free(tmp);
}

void printList(SNode* head)
{
    if(!head)
    {
        printf("Empty list\n");
        return;
    }
    
    SNode* tmp = head;
    while (tmp)
    {
        printf("%d ", tmp->data);
        tmp = tmp->next;
    }
    printf("\n");
}

void freeList(SNode** head)
{
    if(!head)
    {
        fprintf(stderr, "Invalid head pointer\n");
        return;
    }
    
    SNode* tmp;
    while (*head)
    {
        tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
}
