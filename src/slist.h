// slist.h

#ifndef SLIST_H
#define SLIST_H

typedef struct SNode SNode;

SNode *createSNode(int data);
void deleteSNode(SNode **head, int data);
void insertSFront(SNode **head, int data);
void insertSEnd(SNode **head, int data);
void printSList(SNode *head);
void freeSList(SNode **head);

#endif // !SLIST_H
