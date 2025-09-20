#include "test_runner.h"
#include "../tests/test_avl_tree.c"
#include "../tests/test_bubble_sort.c"
#include "../tests/test_dlist.c"
#include "../tests/test_graph.c"
#include "../tests/test_hash_table.c"
#include "../tests/test_merge_sort.c"
#include "../tests/test_quick_sort.c"
#include "../tests/test_slist.c"
#include "../tests/test_vector.c"
#include <stdio.h>

void test_runner(void)
{
    printf("\n[ RUN ] All tests are launched\n");

    printf("\n[ RUN ] Singly Linked List Tests\n\n");
    runSinglyLinkedListTests();
    printf("\n[ PASS ] Singly Linked List Tests\n");

    printf("\n[ RUN ] Doubly Linked List Tests\n\n");
    runDoublyLinkedListTests();
    printf("\n[ PASS ] Doubly Linked List Tests\n");

    printf("\n[ RUN ] Bubble Sort Tests\n\n");
    runBubbleSortTests();
    printf("\n[ PASS ] Bubble Sort Tests\n");

    printf("\n[ RUN ] Vector Tests\n\n");
    runVectorTests();
    printf("\n[ PASS ] Vector Tests\n");

    printf("\n[ RUN ] Hash Table Tests\n\n");
    runHashTableTests();
    printf("\n[ PASS ] Hash Table Tests\n");

    printf("\n[ RUN ] Quicksort Tests\n\n");
    runQuicksortTests();
    printf("\n[ PASS ] Quicksort Tests\n");

    printf("\n[ RUN ] Mergesort Tests\n\n");
    runMergesortTests();
    printf("\n[ PASS ] Mergesort Tests\n");

    printf("\n[ RUN ] AVL Tree Tests\n\n");
    runAVLTreeTests();
    printf("\n[ PASS ] AVL Tree Tests\n");

    printf("\n[ RUN ] Graph Tests\n\n");
    runGraphTests();
    printf("\n[ PASS ] Graph Tests\n");

    printf("\n[ PASS ] All tests completed\n\n");
}
