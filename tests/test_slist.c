#include "../src/slist.h"
#include <limits.h>
#include <stdio.h>

void runSinglyLinkedListTests()
{
    SNode* head = NULL;

    // Test 1: InsertFront
    printf("Test 1: InsertFront 3, 2, 1\n");
    insertFront(&head, 3);
    insertFront(&head, 2);
    insertFront(&head, 1);
    printf("Result: ");
    printList(head); // Expected: 1 2 3

    // Test 2: InsertEnd
    printf("Test 2: InsertEnd 4, 5\n");
    insertEnd(&head, 4);
    insertEnd(&head, 5);
    printf("Result: ");
    printList(head); // Expected: 1 2 3 4 5

    // Test 3: DeleteNode
    printf("Test 3: Delete 2\n");
    deleteNode(&head, 2);
    printf("Result: ");
    printList(head); // Expected: 1 3 4 5

    // Test 4: Delete head (1)
    printf("Test 4: Delete head (1)\n");
    deleteNode(&head, 1);
    printf("Result: ");
    printList(head); // Expected: 3 4 5

    // Test 5: Delete non-existent
    printf("Test 5: Delete non-existent 99\n");
    deleteNode(&head, 99);
    printf("Result: ");
    printList(head); // Expected: 3 4 5

    // Cleanup
    freeList(&head);
    printf("\n");
}
