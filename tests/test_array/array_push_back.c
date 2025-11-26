// #include <assert.h>
// #include <stdio.h>
// #include <stdlib.h>

// #include "../src/array.h"

// static void test_array_push_back(void)
// {
//     Array *a = array_new(sizeof(int));
//     assert(a);

//     int x = 42;
//     assert(array_push_back(a, &x) == 0);
//     assert(a == 1);

//     int *data = (int *)a->data;
//     assert(data[0] == 42);

//     array_delete(a);
// }

// // static void test_array_push_back(void)
// // {
// //     printf("\n[ RUN ] test_array_push_back\n");
// //     Array * array = array_new(sizeof(int));

// //     assert(array);

// //     int a = 42;

// //     assert(array_push_back(array, &a) == 0);

// //     array_delete(array);
// //     printf("[ PASS ] test_array_push_back\n");
// // }

// void run_array_tests_push_back(void)
// {
//     test_array_push_back();
// }
