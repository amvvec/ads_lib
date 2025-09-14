#ifndef QUICK_SORT_H
#define QUICK_SORT_H

/** @brief Sorts an array of integers using the quicksort algorithm.
 *  @param arr Pointer to the integer array to be sorted.
 *  @param low The starting index of the subarray to sort.
 *  @param high The ending index of the subarray to sort.
 *  @note Uses the last element as the pivot. Average time complexity is O(n log n), worst case is O(n²).
 *  @warning Function performs basic validation; invalid inputs (NULL array, negative indices) will trigger an error message and exit early.
 */
void quick_sort(int arr[], int low, int high);

#endif // !QUICK_SORT_H
