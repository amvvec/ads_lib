#include "test_runner.h"
#include "../tests/test_dlist.c"
#include "../tests/test_slist.c"
#include <stdio.h>

void test_runner(void)
{
    printf("[RUN] Singly Linked List Tests\n");
    runSinglyLinkedListTests();
    printf("[PASS] Singly Linked List Tests\n");
    printf("\n[RUN] Doubly Linked List Tests\n");
    runDoublyLinkedListTests();
    printf("[PASS] Doubly Linked List Tests\n");
    printf("\n[PASS] All tests completed\n");
}
