#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>

/** @brief Computes the hash value for a given key.
 *  @param key The integer key to hash.
 *  @param size The number of buckets in the hash table.
 *  @return The hash index (0 to size-1), or 0 if size is invalid.
 *  @note Uses a simple modulo operation. O(1) time complexity.
 */
unsigned int hash_function(int key, int size)
{
    if(size <= 0)
    {
        fprintf(stderr, "Invalid hash table size\n");
        return 0;
    }
    return key % size;
}

/** @brief Initializes a new hash table with the specified number of buckets.
 *  @param size The initial number of buckets in the hash table.
 *  @return Pointer to the new HashTable, or NULL if initialization fails
 * (invalid size or memory allocation error).
 *  @note Sets all buckets to NULL. O(n) time complexity, where n is the size.
 */
HashTable * hash_table_init(int size)
{
    if(size <= 0)
    {
        fprintf(stderr, "Invalid initial size for hash table\n");
        return NULL;
    }
    HashTable * ht = (HashTable *)malloc(sizeof(HashTable));
    if(!ht)
    {
        fprintf(stderr, "Memory allocation failed for HashTable\n");
        return NULL;
    }
    ht->buckets = (Node **)malloc(size * sizeof(Node *));
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

/** @brief Inserts or updates a key-value pair in the hash table.
 *  @param ht Pointer to the HashTable.
 *  @param key The integer key to insert or update.
 *  @param value The integer value to associate with the key.
 *  @note Updates the value if the key already exists; otherwise, adds a new
 * node. O(1) average time complexity.
 */
void hash_table_put(HashTable * ht, int key, int value)
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
    Node * new_node = (Node *)malloc(sizeof(Node));
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

/** @brief Retrieves the value associated with the specified key.
 *  @param ht Pointer to the HashTable.
 *  @param key The integer key to look up.
 *  @return The value associated with the key, or -1 if the key is not found.
 *  @note O(1) average time complexity, O(n) in worst case due to collisions.
 */
int hash_table_get(HashTable * ht, int key)
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
    Node * current = ht->buckets[index];
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

/** @brief Frees the memory allocated for the hash table.
 *  @param ht Pointer to the HashTable.
 *  @note Frees all nodes in each bucket and the hash table itself. O(n) time
 * complexity, where n is the total number of nodes.
 */
void hash_table_free(HashTable * ht)
{
    if(!ht)
    {
        fprintf(stderr, "Null HashTable pointer\n");
        return;
    }
    int size = ht->size;
    for(int i = 0; i < size; i++)
    {
        Node * current = ht->buckets[i];
        while(current)
        {
            Node * temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(ht->buckets);
    free(ht);
}
