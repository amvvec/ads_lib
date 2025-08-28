#include "../src/algorithm/bubble_sort.h"
#include <stdio.h>

/** @brief Runs test suite for the bubble sort algorithm.
 *  @note Tests sorting, empty array, single-element array, and sorted array.
 */
void runBubbleSortTests()
{
    // Test 1: Basic sorting
    printf("Test 1: Basic sorting [5, 3, 8, 1, 2]\n");
    int arr1[] = {5, 3, 8, 1, 2};
    int size1 = 5;
    bubble_sort(arr1, size1);
    printf("Result: ");
    for(int i = 0; i < size1; i++)
    {
        printf("%d ", arr1[i]);
    }
    printf("\n"); // Expected: 1 2 3 5 8

    // Test 2: Null array
    printf("Test 2: Null array\n");
    bubble_sort(NULL, 0);
    printf("Result: Null array\n"); // Expected: Null array (no-op)

    // Test 3: Single-element array
    printf("Test 3: Single-element array [42]\n");
    int arr3[] = {42};
    bubble_sort(arr3, 1);
    printf("Result: %d\n", arr3[0]); // Expected: 42

    // Test 4: Already sorted array
    printf("Test 4: Already sorted array [1, 2, 3, 4, 5]\n");
    int arr4[] = {1, 2, 3, 4, 5};
    bubble_sort(arr4, 5);
    printf("Result: ");
    for(int i = 0; i < 5; i++)
    {
        printf("%d ", arr4[i]);
    }
    printf("\n"); // Expected: 1 2 3 4 5

    printf("\n");
}
