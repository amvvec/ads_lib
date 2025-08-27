// test_runner.h

#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include "../tests/test_slist.c"

#include <stdio.h>

void test_runner()
{
    printf("\n[RUN] Singly Linked List Tests\n\n");
    runSinglyLinkedListTests();
    printf("[PASS] Singly Linked List Tests\n\n");
}

#endif // !TEST_RUNNER_H
