#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../tests/test_runner.c"

int main(void)
{
    srand((unsigned)time(NULL));
    test_runner();
    return 0;
}
