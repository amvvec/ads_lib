#include "bubble_sort.h"
#include "test_runner.h"

#include <stdio.h>

int main(void)
{
    test_runner();

    int arr[] = {5, 3, 8, 1, 2};
    int size = 5;

    printf("Original array: ");
    for(int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    bubble_sort(arr, size);

    printf("Sorted array: ");
    for(int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
