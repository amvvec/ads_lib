#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "skip_list.h"

#define P 0.5

static int random_level(void)
{
    int level = 0;
    while(rand() / (double)RAND_MAX < P && level < MAX_LEVEL - 1)
    {
        level++;
    }
    return level;
}

SkipList * create_skip_list(void)
{
    SkipList * list = malloc(sizeof(SkipList));
    if(!list)
    {
        fprintf(stderr, "Skip list allocation failed\n");
        exit(1);
    }
    list->header = malloc(sizeof(SkipNode));
    if(!list->header)
    {
        fprintf(stderr, "Header allocation failed\n");
        free(list);
        exit(1);
    }
    list->header->key = -1;
    for(int i = 0; i < MAX_LEVEL; i++)
    {
        list->header->forward[i] = NULL;
    }
    list->level = 0;
    return list;
}

void insert_skip_list(SkipList * list, int key)
{
    if(!list || !list->header)
    {
        fprintf(stderr, "Invalid skip list\n");
        return;
    }
    SkipNode * update[MAX_LEVEL];
    SkipNode * current = list->header;
    for(int i = list->level; i >= 0; i--)
    {
        while(current->forward[i] && current->forward[i]->key < key)
        {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    if(current && current->key == key)
    {
        return;
    }
    int new_level = random_level();
    if(new_level > list->level)
    {
        for(int i = list->level + 1; i <= new_level; i++)
        {
            update[i] = list->header;
        }
        list->level = new_level;
    }
    SkipNode * new_node = malloc(sizeof(SkipNode));
    if(!new_node)
    {
        fprintf(stderr, "Node allocation failed\n");
        exit(1);
    }
    new_node->key = key;
    for(int i = 0; i <= new_level; i++)
    {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }
}

int search_skip_list(SkipList * list, int key)
{
    if(!list || !list->header)
    {
        fprintf(stderr, "Invalid skip list\n");
        return 0;
    }
    SkipNode * current = list->header;
    for(int i = list->level; i >= 0; i--)
    {
        while(current->forward[i] && current->forward[i]->key < key)
        {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    return current && current->key == key;
}

void delete_skip_list(SkipList * list, int key)
{
    if(!list || !list->header)
    {
        fprintf(stderr, "Invalid skip list\n");
        return;
    }
    SkipNode * update[MAX_LEVEL];
    SkipNode * current = list->header;
    for(int i = list->level; i >= 0; i--)
    {
        while(current->forward[i] && current->forward[i]->key < key)
        {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    if(current && current->key == key)
    {
        for(int i = 0; i <= list->level; i++)
        {
            if(update[i]->forward[i] != current)
            {
                break;
            }
            update[i]->forward[i] = current->forward[i];
        }
        free(current);
        while(list->level > 0 && !list->header->forward[list->level])
        {
            list->level--;
        }
    }
}

void free_skip_list(SkipList * list)
{
    if(!list)
    {
        return;
    }
    SkipNode * current = list->header->forward[0];
    while(current)
    {
        SkipNode * next = current->forward[0];
        free(current);
        current = next;
    }
    if(list->header)
    {
        free(list->header);
    }
    free(list);
}
