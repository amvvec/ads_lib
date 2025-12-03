#include "bubble_sort.h"

#include <limits.h>
#include <stdio.h>

/** @brief Sorts an array of integers in ascending order using bubble sort.
 *  @param arr Pointer to the array to sort.
 *  @param size Number of elements in the array.
 *  @note Handles null array pointer and invalid size by printing error to
 * stderr and returning. O(n^2) time complexity.
 */
void bubble_sort(int arr[], int size)
{
    if(!arr)
    {
        fprintf(stderr, "Null array pointer\n");
        return;
    }
    if(size <= 0)
    {
        fprintf(stderr, "Invalid array size\n");
        return;
    }

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size - 1; j++)
        {
            if((arr + j + 1) >= (arr + size))
            {
                fprintf(stderr, "Buffer overflow detected at index %d\n", j);
                return;
            }
            if(arr[j] > INT_MAX / 2 || arr[j + 1] > INT_MAX / 2)
            {
                fprintf(stderr, "Potential injection detected at index %d\n", j);
                return;
            }

            if(arr[j] > arr[j + 1])
            {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}
