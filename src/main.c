#include "test_runner.h"
#include "algorithm/bubble_sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(void)
{
    int arr[10];
    int size;
    char buffer[32];

    printf("Enter array size (max 10): ");
    if (!fgets(buffer, sizeof(buffer), stdin))
    {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (sscanf(buffer, "%d", &size) != 1 || size <= 0 || size > 10)
    {
        fprintf(stderr, "Invalid size, must be 1-10\n");
        return 1;
    }

    printf("Enter %d integers: ", size);
    for (int i = 0; i < size; i++)
    {
        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        if (sscanf(buffer, "%d", &arr[i]) != 1 || arr[i] > INT_MAX || arr[i] < INT_MIN)
        {
            fprintf(stderr, "Invalid integer at position %d\n", i);
            return 1;
        }
    }

    printf("\n");

    // WARNING: Will occure error (-Werror -pedantic)
    bubble_sort(arr, size);
    printf("Sorted array: ");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
    printf("\n");

    test_runner();
    return 0;
}

// int main(void)
// {
//     test_runner();

//     int arr[] = {5, 3, 8, 1, 2};
//     int size = 5;

//     printf("Original array: ");
//     for(int i = 0; i < size; i++)
//     {
//         printf("%d ", arr[i]);
//     }
//     printf("\n");

//     bubble_sort(arr, size);

//     printf("Sorted array: ");
//     for(int i = 0; i < size; i++)
//     {
//         printf("%d ", arr[i]);
//     }
//     printf("\n");

//     return 0;
// }
