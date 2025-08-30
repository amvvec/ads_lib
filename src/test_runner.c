#include "test_runner.h"
#include "../tests/test_bubble_sort.c"
#include "../tests/test_slist.c"
#include "../tests/test_dlist.c"
#include "../tests/test_vector.c"
#include <stdio.h>

void test_runner(void)
{
    printf("\n");

    printf("[ RUN ] Singly Linked List Tests\n");
    runSinglyLinkedListTests();
    printf("[ PASS ] Singly Linked List Tests\n");
    printf("\n[ RUN ] Doubly Linked List Tests\n");
    runDoublyLinkedListTests();
    printf("[ PASS ] Doubly Linked List Tests\n");
    printf("\n[ RUN ] Bubble Sort Tests\n");
    runBubbleSortTests();
    printf("[ PASS ] Bubble Sort Tests\n");
    printf("\n[ RUN ] Vector Tests\n");
    runVectorTests();
    printf("[ PASS ] Vector Tests\n");
    printf("\n[ PASS ] All tests completed\n");

    printf("\n");
}
