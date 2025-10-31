#include <stdio.h>

#include "../src/hash_table.h"

void runHashTableTests(void)
{
    printf("\n[ RUN ] Hash Table Tests\n\n");
    // Test 1: Initialization
    printf("Test 1: Initialize Hash Table with size 5\n");
    HashTable * ht = hash_table_init(5);
    if(ht && ht->size == 5)
    {
        printf("Result: Initialized successfully\n");
    }
    else
    {
        printf("Result: Initialization failed\n");
    }

    // Test 2: Put and Get
    printf("Test 2: Put key-value pairs (1:10, 2:20, 3:30)\n");
    hash_table_put(ht, 1, 10);
    hash_table_put(ht, 2, 20);
    hash_table_put(ht, 3, 30);
    if(hash_table_get(ht, 1) == 10 && hash_table_get(ht, 2) == 20 &&
       hash_table_get(ht, 3) == 30)
    {
        printf("Result: Get successful (1:10, 2:20, 3:30)\n");
    }
    else
    {
        printf("Result: Put or Get failed\n");
    }

    // Test 3: Get non-existent key
    printf("Test 3: Get non-existent key (4)\n");
    if(hash_table_get(ht, 4) == -1)
    {
        printf("Result: Returned -1 for non-existent key\n");
    }
    else
    {
        printf("Result: Get failed for non-existent key\n");
    }

    // Test 4: Free
    printf("Test 4: Free Hash Table\n");
    hash_table_free(ht);
    printf("Result: Freed\n");
    printf("\n[ PASS ] Hash Table Tests\n");
}
