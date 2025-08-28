#include "../src/data_structure/dlist.h"
#include <stdio.h>

void runDoublyLinkedListTests()
{
    DNode* head = NULL;

    // Test 1: InsertDFront
    printf("Test 1: InsertDFront 3, 2, 1\n");
    insertDFront(&head, 3);
    insertDFront(&head, 2);
    insertDFront(&head, 1);
    printf("Result: ");
    printDList(head); // Expected: 1 2 3

    // Test 2: InsertDEnd
    printf("Test 2: InsertDEnd 4, 5\n");
    insertDEnd(&head, 4);
    insertDEnd(&head, 5);
    printf("Result: ");
    printDList(head); // Expected: 1 2 3 4 5

    // Test 3: DeleteDNode
    printf("Test 3: Delete 2\n");
    deleteDNode(&head, 2);
    printf("Result: ");
    printDList(head); // Expected: 1 3 4 5

    // Test 4: Delete head (1)
    printf("Test 4: Delete head (1)\n");
    deleteDNode(&head, 1);
    printf("Result: ");
    printDList(head); // Expected: 3 4 5

    // Test 5: Delete non-existent
    printf("Test 5: Delete non-existent 99\n");
    deleteDNode(&head, 99);
    printf("Result: ");
    printDList(head); // Expected: 3 4 5

    // Cleanup
    freeDList(&head);
    printf("\n");
}
