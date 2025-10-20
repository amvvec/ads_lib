#include <stdio.h>

#include "../src/binary_search.h"

void runBinarySearchTests(void)
{
    printf("\n[ RUN ] Binary Search Tests\n\n");
    // Test 1: Target in middle
    printf("Test 1: Target in middle (7 in [1,3,5,7,9])\n");
    int arr1[] = {1, 3, 5, 7, 9};
    int result1 = binary_search(arr1, 5, 7);
    if(result1 == 3)
    {
        printf("Result: Pass (found at index %d)\n", result1);
    }
    else
    {
        printf("Result: Fail (expected 3, got %d)\n", result1);
    }

    // Test 2: Target at start
    printf("Test 2: Target at start (1 in [1,3,5,7,9])\n");
    int result2 = binary_search(arr1, 5, 1);
    if(result2 == 0)
    {
        printf("Result: Pass (found at index %d)\n", result2);
    }
    else
    {
        printf("Result: Fail (expected 0, got %d)\n", result2);
    }

    // Test 3: Target not found
    printf("Test 3: Target not found (4 in [1,3,5,7,9])\n");
    int result3 = binary_search(arr1, 5, 4);
    if(result3 == -1)
    {
        printf("Result: Pass (not found)\n");
    }
    else
    {
        printf("Result: Fail (expected -1, got %d)\n", result3);
    }

    // Test 4: Empty array
    printf("Test 4: Empty array\n");
    int result4 = binary_search(NULL, 0, 5);
    if(result4 == -1)
    {
        printf("Result: Pass (handled empty array)\n");
    }
    else
    {
        printf("Result: Fail (expected -1, got %d)\n", result4);
    }

    // Test 5: Single element
    printf("Test 5: Single element (5 in [5])\n");
    int arr5[] = {5};
    int result5 = binary_search(arr5, 1, 5);
    if(result5 == 0)
    {
        printf("Result: Pass (found at index %d)\n", result5);
    }
    else
    {
        printf("Result: Fail (expected 0, got %d)\n", result5);
    }
    printf("\n[ PASS ] Binary Search Tests\n");
}
