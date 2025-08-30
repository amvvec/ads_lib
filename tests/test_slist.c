#include "../src/data_structure/slist.h"
#include <stdio.h>

void runSinglyLinkedListTests()
{
    SNode* head = NULL;

    // Test 1: InsertFront
    printf("\nTest 1: InsertFront 3, 2, 1\n");
    insertSFront(&head, 3);
    insertSFront(&head, 2);
    insertSFront(&head, 1);
    printf("Result: ");
    printSList(head); // Expected: 1 2 3

    // Test 2: InsertEnd
    printf("Test 2: InsertEnd 4, 5\n");
    insertSEnd(&head, 4);
    insertSEnd(&head, 5);
    printf("Result: ");
    printSList(head); // Expected: 1 2 3 4 5

    // Test 3: DeleteNode
    printf("Test 3: Delete 2\n");
    deleteSNode(&head, 2);
    printf("Result: ");
    printSList(head); // Expected: 1 3 4 5

    // Test 4: Delete head (1)
    printf("Test 4: Delete head (1)\n");
    deleteSNode(&head, 1);
    printf("Result: ");
    printSList(head); // Expected: 3 4 5

    // Test 5: Delete non-existent
    printf("Test 5: Delete non-existent 99\n");
    deleteSNode(&head, 99);
    printf("Result: ");
    printSList(head); // Expected: 3 4 5

    // Cleanup
    freeSList(&head);
    printf("\n");
}
