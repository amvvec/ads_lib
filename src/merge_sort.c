#include "merge_sort.h"

#include <stdio.h>

/** @brief Merges two sorted subarrays into a single sorted array.
 *  @param arr Pointer to the integer array.
 *  @param low The starting index of the first subarray.
 *  @param mid The ending index of the first subarray.
 *  @param high The ending index of the second subarray.
 *  @note Combines two sorted segments. Time complexity is O(n).
 *  @warning Validates input parameters; invalid cases (NULL array, negative indices, invalid mid) trigger an error message and exit early.
 */
void merge(int arr[], int low, int mid, int high)
{
    if(!arr || low < 0 || mid < 0 || high < 0 || low > mid || mid > high)
    {
        fprintf(stderr, "Invalid merge parameters: arr:%p\n low:%d\n mid=%d\n high=%d\n", (void*)arr, low, mid, high);
        return;
    }

    int n1 = mid - low + 1;
    int n2 = high - mid;
    int L[n1], R[n2];

    for(int i = 0; i < n1; i++)
    {
        L[i] = arr[low + i];
    }
    for(int j = 0; j < n2; j++)
    {
        R[j] = arr[mid + 1 + j];
    }
    int i = 0, j = 0, k = low;
    while(i < n1 && j < n2)
    {
        if(L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while(i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    while(j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/** @brief Sorts an array of integers using the mergesort algorithm.
 *  @param arr Pointer to the integer array to be sorted.
 *  @param low The starting index of the subarray to sort.
 *  @param high The ending index of the subarray to sort.
 *  @note Uses a divide-and-conquer approach with merging. Time complexity is O(n log n) in all cases.
 *  @warning Function performs basic validation; invalid inputs (NULL array, negative indices) will trigger an error message and exit early.
 */
void merge_sort(int arr[], int low, int high)
{
    if(!arr || low < 0 || high < 0)
    {
        fprintf(stderr, "Invalid merge sort parameters: arr:%p\n low:%d\n high=%d\n", (void*)arr, low, high);
        return;
    }
    if(low > high)
    {
        return;
    }
    if(low < high)
    {
        int mid = low + (high - low) / 2;
        merge_sort(arr, low, mid);
        merge_sort(arr, mid + 1, high);
        merge(arr, low, mid, high);
    }
}
