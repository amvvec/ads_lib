#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct Node
{
    int key;
    int value;
    struct Node* next;
} Node;

typedef struct
{
    Node** buckets;
    int size;
} HashTable;

HashTable* hash_table_init(int size);
void hash_table_put(HashTable* ht, int key, int value);
int hash_table_get(HashTable* ht, int key);
void hash_table_free(HashTable* ht);

#endif // HASH_TABLE_H