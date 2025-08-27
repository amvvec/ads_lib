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
    DNode* node = (DNode*)malloc(sizeof(DNode));
    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void insertDFront(DNode** head, int data)
{
    DNode* newNode = createDNode(data);
    newNode->next = *head;
    if(*head)
    {
        (*head)->prev = newNode;
    }
    *head = newNode;
}

void insertDEnd(DNode** head, int data)
{
    DNode* newNode = createDNode(data);
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
    DNode* tmp = *head;
    if(tmp && tmp->data == data)
    {
        *head = tmp->next;
        if(*head)
        {
            (*head)->prev = NULL;
        }
        free(tmp);
        return;
    }
    while(tmp && tmp->data != data)
    {
        tmp = tmp->next;
    }
    if(!tmp)
    {
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
}

void printDList(DNode* head)
{
    DNode* tmp = head;
    while(tmp)
    {
        printf("%d ", tmp->data);
        tmp = tmp->next;
    }
    printf("\n");
}

void freeDList(DNode** head)
{
    DNode* tmp;
    while(*head)
    {
        tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
}
