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
    SNode* newNode = createNode(data);
    newNode->next = *head;
    *head = newNode;
}

void insertEnd(SNode** head, int data)
{
    SNode* newNode = createNode(data);
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
        return;
    }
    prev->next = tmp->next;
    free(tmp);
}

void printList(SNode* head)
{
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
    SNode* tmp;
    while (*head)
    {
        tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
}
