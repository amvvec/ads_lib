#include "test_runner.h"
#include "../tests/test_slist.c"

#include <stdio.h>

void test_runner(void)
{
    printf("\n[RUN] Singly Linked List Tests\n\n");
    runSinglyLinkedListTests();
    printf("[PASS] Singly Linked List Tests\n\n");
}
