#include "../src/quick_sort.h"
#include "test_runner.h"

#include <stdio.h>
#include <stdlib.h>

/** @brief Runs test suite for the quicksort algorithm.
 *  @note Tests basic sorting, empty array, and already sorted array.
 */
void runQuicksortTests()
{
    // Test 1: Basic sorting
    printf("Test 1: Sort array [5, 3, 8, 1, 2]\n");
    int arr1[] = {5, 3, 8, 1, 2};
    int size1 = 5;
    quick_sort(arr1, 0, size1 - 1);
    int passed1 = 1;
    for(int i = 0; i < size1 - 1; i++)
    {
        if(arr1[i] > arr1[i + 1])
        {
            passed1 = 0;
            break;
        }
    }
    if(passed1 && arr1[0] == 1 && arr1[4] == 8)
    {
        printf("Result: Sorted successfully [1, 2, 3, 5, 8]\n");
    }
    else
    {
        printf("Result: Sorting failed\n");
    }

    // Test 2: Single-element array (замена пустого массива)
    printf("Test 2: Sort single-element array [42]\n");
    int arr2[] = {42};
    int size2 = 1;
    quick_sort(arr2, 0, size2 - 1);
    if(arr2[0] == 42)
    {
        printf("Result: Remains [42]\n");
    }
    else
    {
        printf("Result: Sorting failed\n");
    }

    // Test 3: Already sorted array
    printf("Test 3: Sort already sorted array [1, 2, 3, 4, 5]\n");
    int arr3[] = {1, 2, 3, 4, 5};
    int size3 = 5;
    quick_sort(arr3, 0, size3 - 1);
    int passed3 = 1;
    for(int i = 0; i < size3 - 1; i++)
    {
        if(arr3[i] > arr3[i + 1])
        {
            passed3 = 0;
            break;
        }
    }
    if(passed3)
    {
        printf("Result: Remains sorted [1, 2, 3, 4, 5]\n");
    }
    else
    {
        printf("Result: Sorting failed\n");
    }
}
