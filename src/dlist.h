#ifndef DLIST_H
#define DLIST_H

typedef struct DNode DNode;

DNode* createDNode(int data);
void deleteDNode(DNode** head, int data);
void insertDFront(DNode** head, int data);
void insertDEnd(DNode** head, int data);
void printDList(DNode* head);
void freeDList(DNode** head);

#endif // !DLIST_H
