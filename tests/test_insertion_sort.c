#include "../src/insertion_sort.h"
#include <stdio.h>
#include <stdlib.h>

void runInsertionSortTests() {
    // Test 1: Random array
    printf("Test 1: Sort [5, 2, 8, 1, 9]\n");
    int arr1[] = {5, 2, 8, 1, 9};
    int expected1[] = {1, 2, 5, 8, 9};
    insertion_sort(arr1, 5);
    int pass1 = 1;
    for (int i = 0; i < 5; i++)
        if (arr1[i] != expected1[i]) pass1 = 0;
    printf("Result: %s\n", pass1 ? "Pass" : "Fail");

    // Test 2: Already sorted
    printf("Test 2: Sort [1, 2, 3, 4]\n");
    int arr2[] = {1, 2, 3, 4};
    int expected2[] = {1, 2, 3, 4};
    insertion_sort(arr2, 4);
    int pass2 = 1;
    for (int i = 0; i < 4; i++)
        if (arr2[i] != expected2[i]) pass2 = 0;
    printf("Result: %s\n", pass2 ? "Pass" : "Fail");

    // Test 3: Invalid input (null)
    printf("Test 3: Null array\n");
    int result3 = insertion_sort(NULL, 5);
    printf("Result: %s\n", result3 == -1 ? "Pass" : "Fail");

    // Test 4: Empty array
    printf("Test 4: Empty array (size=0)\n");
    int result4 = insertion_sort(arr1, 0);
    printf("Result: %s\n", result4 == -1 ? "Pass" : "Fail");

    // Test 5: Single element
    printf("Test 5: Single element [42]\n");
    int arr5[] = {42};
    int expected5[] = {42};
    insertion_sort(arr5, 1);
    int pass5 = arr5[0] == expected5[0];
    printf("Result: %s\n", pass5 ? "Pass" : "Fail");
}
