// slist.h

#ifndef SLIST_H
#define SLIST_H

typedef struct SNode SNode;

SNode * createNode(int data);
void deleteNode(SNode ** head, int data);

void insertFront(SNode ** head, int data);
void insertEnd(SNode ** head, int data);

void printList(SNode * head);
void freeList(SNode ** head);

#endif // !SLIST_H
