#include <stdio.h>
#include <stdlib.h>

#include "insertion_sort.h"

/**
 * @brief Sorts an array using insertion sort.
 * @param arr Array to sort.
 * @param size Size of the array.
 * @return 0 on success, -1 on error (null array or invalid size).
 * @complexity O(n²) worst/average case, O(n) best case.
 */
int insertion_sort(int *arr, int size)
{
    if(!arr)
    {
        fprintf(stderr, "Null pointer\n");
        return -1;
    }
    for(int i = 0; i < size; i++)
    {
        int key = arr[i];
        int j = i - 1;
        while(j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
    return 0;
}
