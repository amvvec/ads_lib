#include <stdio.h>

#include "binary_search.h"

int binary_search(int* arr, int size, int target)
{
    if(!arr || size <= 0)
    {
        fprintf(stderr, "Invalid array or size\n");
        return -1;
    }
    
    int left = 0;
    int right = size - 1;
    while(left <= right)
    {
        int mid = left + (right - left) / 2;
        if(arr[mid] == target)
        {
            return mid;
        }
        if(arr[mid] < target)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return -1;
}
