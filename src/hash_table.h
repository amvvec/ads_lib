#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/** @struct Node
 *  @brief A node in the linked list used for collision resolution in the hash
 * table.
 *  @var Node::key The integer key for the hash table entry.
 *  @var Node::value The integer value associated with the key.
 *  @var Node::next Pointer to the next node in the chain (for collision
 * handling).
 */
typedef struct Node
{
    int key;            /**< The integer key for the hash table entry. */
    int value;          /**< The integer value associated with the key. */
    struct Node * next; /**< Pointer to the next node in the chain (for
                           collision handling). */
} Node;

/** @struct HashTable
 *  @brief A hash table implementation using chaining for collision resolution.
 *  @var HashTable::buckets Array of pointers to Node, representing the hash
 * buckets.
 *  @var HashTable::size Number of buckets in the hash table.
 */
typedef struct
{
    Node ** buckets; /**< Array of pointers to Node, representing the hash
                        buckets. */
    int size;        /**< Number of buckets in the hash table. */
} HashTable;

HashTable * hash_table_init(int size);
void hash_table_put(HashTable * ht, int key, int value);
int hash_table_get(HashTable * ht, int key);
void hash_table_free(HashTable * ht);

#endif // HASH_TABLE_H