#include "quick_sort.h"

#include <stdio.h>

void swap(int *a, int *b)
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

int partition(int arr[], int low, int high)
{
    if(!arr || low < 0 || high < 0 || low > high)
    {
        fprintf(stderr, "Invalid partition parameters: arr=%p, low=%d, high=%d\n", (void *)arr, low, high);
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

void quick_sort(int arr[], int low, int high)
{
    if (!arr || low < 0 || high < 0)
    {
        fprintf(stderr, "Invalid quicksort parameters: arr=%p, low=%d, high=%d\n", (void*)arr, low, high);
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
