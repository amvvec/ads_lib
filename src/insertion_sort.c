

#include <stdio.h>
#include <stdlib.h>

#include "insertion_sort.h"

int insertion_sort(int* arr, int size)
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
