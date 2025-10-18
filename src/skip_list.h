#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#define MAX_LEVEL 16

typedef struct SkipNode
{
    int key;
    struct SkipNode* forward[MAX_LEVEL]; // Array of pointers to next nodes
} SkipNode;

typedef struct SkipList
{
    SkipNode* header;
    int level; // Current max level
} SkipList;

SkipList* create_skip_list(void);
void free_skip_list(SkipList* list);
void insert_skip_list(SkipList* list, int key);
int search_skip_list(SkipList* list, int key);
void delete_skip_list(SkipList* list, int key);

#endif // !SKIP_LIST_H
