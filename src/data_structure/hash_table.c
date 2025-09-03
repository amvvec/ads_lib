#include <stdlib.h>
#include "hash_table.h"
#include <stdlib.h>

unsigned int hash_function(int key, int size)
{
    return key % size;
}

HashTable* hash_table_init(int size)
{
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    ht->buckets = (Node**)malloc(size * sizeof(Node*));
    ht->size = size;
    for(int i = 0; i < size; i++)
    {
        ht->buckets[i] = NULL;
    }
    return ht;
}

void hash_table_put(HashTable* ht, int key, int value)
{
    unsigned int index = hash_function(key, ht->size);
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->key = key;
    new_node->value = value;
    new_node->next = ht->buckets[index];
    ht->buckets[index] = new_node;
}

int hash_table_get(HashTable* ht, int key)
{
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
