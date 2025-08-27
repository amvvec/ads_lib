#include "test_runner.h"
#include "../tests/test_slist.c"
// #include "../tests/test_dlist.c"

#include <stdio.h>

/** @brief Runs all test suites for the data structures.
 *  @note Executes tests for singly and doubly linked lists, printing results
 * with [RUN] and [PASS].
 */
void test_runner(void)
{
    printf("\n[RUN] Singly Linked List Tests\n\n");
    runSinglyLinkedListTests();
    printf("[PASS] Singly Linked List Tests\n\n");
}
