#include "quick_sort.h"

#include <stdio.h>

/** @brief Swaps two integer values.
 *  @param a Pointer to the first integer.
 *  @param b Pointer to the second integer.
 *  @note Does nothing if either pointer is NULL. Time complexity is O(1).
 *  @warning Null pointer check is performed to prevent undefined behavior.
 */
void swap(int* a, int* b)
{
    if(!a || !b)
    {
        fprintf(stderr, "Null pointer in swap\n");
        return;
    }

    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/** @brief Swaps two integer values.
 *  @param a Pointer to the first integer.
 *  @param b Pointer to the second integer.
 *  @note Does nothing if either pointer is NULL. Time complexity is O(1).
 *  @warning Null pointer check is performed to prevent undefined behavior.
 */
int partition(int arr[], int low, int high)
{
    if(!arr || low < 0 || high < 0 || low > high)
    {
        fprintf(stderr,
                "Invalid partition parameters: arr=%p, low=%d, high=%d\n",
                (void*)arr, low, high);
        return -1;
    }

    int pivot = arr[high];

    int i = low - 1;
    for(int j = low; j < high; j++)
    {
        if(arr[j] <= pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

/** @brief Sorts an array of integers using the quicksort algorithm.
 *  @param arr Pointer to the integer array to be sorted.
 *  @param low The starting index of the subarray to sort.
 *  @param high The ending index of the subarray to sort.
 *  @note Uses the last element as the pivot. Average time complexity is O(n log
 * n), worst case is O(n²).
 *  @warning Function performs basic validation; invalid inputs (NULL array,
 * negative indices) will trigger an error message and exit early.
 */
void quick_sort(int arr[], int low, int high)
{
    if(!arr || low < 0 || high < 0)
    {
        fprintf(stderr,
                "Invalid quicksort parameters: arr=%p, low=%d, high=%d\n",
                (void*)arr, low, high);
        return;
    }
    if(low >= high)
    {
        return;
    }

    if(low < high)
    {
        int pi = partition(arr, low, high);
        if(pi != -1)
        {
            quick_sort(arr, low, pi - 1);
            quick_sort(arr, pi + 1, high);
        }
    }
}
