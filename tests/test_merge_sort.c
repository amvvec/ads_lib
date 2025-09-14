#include "../src/merge_sort.h"

#include <stdio.h>
#include <stdlib.h>

void runMergesortTests()
{
    // Test 1: Basic sorting
    printf("Test 1: Sort array [5, 3, 8, 1, 2]\n");
    int arr1[] = {5, 3, 8, 1, 2};
    int size1 = 5;
    merge_sort(arr1, 0, size1 - 1);
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

    // Test 2: Already sorted array
    printf("Test 2: Sort already sorted array [1, 2, 3, 4, 5]\n");
    int arr2[] = {1, 2, 3, 4, 5};
    int size2 = 5;
    merge_sort(arr2, 0, size2 - 1);
    int passed2 = 1;
    for(int i = 0; i < size2 - 1; i++)
    {
        if(arr2[i] > arr2[i + 1])
        {
            passed2 = 0;
            break;
        }
    }
    if(passed2)
    {
        printf("Result: Remains sorted [1, 2, 3, 4, 5]\n");
    }
    else
    {
        printf("Result: Sorting failed\n");
    }
}
