#include <stdio.h>
#include <stdlib.h>

#include "../src/skip_list.h"

void runSkipListTests()
{
    // Test 1: Insert and search
    printf("Test 1: Insert [5, 2, 8] and search\n");
    SkipList* list = create_skip_list();
    if(!list)
    {
        printf("Result: Fail (creation failed)\n");
        return;
    }
    insert_skip_list(list, 5);
    insert_skip_list(list, 2);
    insert_skip_list(list, 8);
    int pass1 = search_skip_list(list, 5) && search_skip_list(list, 2) &&
                search_skip_list(list, 8) && !search_skip_list(list, 3);
    printf("Result: %s\n", pass1 ? "Pass" : "Fail");

    // Test 2: Delete
    printf("Test 2: Delete 5 and search\n");
    delete_skip_list(list, 5);
    int pass2 = !search_skip_list(list, 5) && search_skip_list(list, 2) &&
                search_skip_list(list, 8);
    printf("Result: %s\n", pass2 ? "Pass" : "Fail");

    // Test 4: Empty list search
    printf("Test 4: Search in empty list\n");
    SkipList* empty_list = create_skip_list();
    if(!empty_list)
    {
        printf("Result: Fail (creation failed)\n");
        free_skip_list(list);
        return;
    }
    int pass4 = !search_skip_list(empty_list, 1);
    printf("Result: %s\n", pass4 ? "Pass" : "Fail");

    // Test 5: Memory safety (insert, delete, free)
    printf("Test 5: Insert, delete, and free\n");
    insert_skip_list(empty_list, 10);
    insert_skip_list(empty_list, 20);
    delete_skip_list(empty_list, 10);
    int pass5 =
        search_skip_list(empty_list, 20) && !search_skip_list(empty_list, 10);
    free_skip_list(empty_list);
    printf("Result: %s\n", pass5 ? "Pass" : "Fail");

    free_skip_list(list);
}
