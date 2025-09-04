#include "hash_table.h"
#include <stdlib.h>
#include <stdio.h>

unsigned int hash_function(int key, int size)
{
    if(size <= 0)
    {
        fprintf(stderr, "Invalid hash table size\n");
        return 0;
    }
    return key % size;
}

HashTable* hash_table_init(int size)
{
    if(size <= 0)
    {
        fprintf(stderr, "Invalid initial size for hash table\n");
        return NULL;
    }
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if(!ht)    
    {
        fprintf(stderr, "Memory allocation failed for HashTable\n");
        return NULL;
    }
    ht->buckets = (Node**)malloc(size * sizeof(Node*));
    if(!ht->buckets)
    {
        fprintf(stderr, "Memory allocation failed for buckets\n");
        free(ht);
        return NULL;
    }
    ht->size = size;
    for(int i = 0; i < size; i++)
    {
        ht->buckets[i] = NULL;
    }
    return ht;
}

void hash_table_put(HashTable* ht, int key, int value)
{
    if(!ht)
    {
        fprintf(stderr, "Null HashTable pointer\n");
        return;
    }
    if(ht->size <= 0)
    {
        fprintf(stderr, "Invalid hash table size\n");
        return;
    }
    unsigned int index = hash_function(key, ht->size);
    Node * current = ht->buckets[index];
    while(current)
    {
        if(current->key == key)
        {
            current->value = value;
            return;
        }
    }
    Node* new_node = (Node*)malloc(sizeof(Node));
    if(!new_node)
    {
        fprintf(stderr, "Memory allocation failed for new node\n");
        return;
    }
    new_node->key = key;
    new_node->value = value;
    new_node->next = ht->buckets[index];
    ht->buckets[index] = new_node;
}

int hash_table_get(HashTable* ht, int key)
{
    if(!ht)
    {
        fprintf(stderr, "Null HashTable pointer\n");
        return -1;
    }
    if(ht->size <= 0)
    {
        fprintf(stderr, "Invalid hash table size\n");
        return -1;
    }
    unsigned int index = hash_function(key, ht->size);
    Node* current = ht->buckets[index];
    while(current)
    {
        if(current->key == key)
        {
            return current->value;
        }
        current = current->next;
    }
    return -1; // Default return if there is no key
}

void hash_table_free(HashTable* ht)
{
    if(!ht)
    {
        fprintf(stderr, "Null HashTable pointer\n");
        return;
    }
    int size = ht->size;
    for(int i = 0; i < size; i++)
    {
        Node* current = ht->buckets[i];
        while(current)
        {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(ht->buckets);
    free(ht);
}
