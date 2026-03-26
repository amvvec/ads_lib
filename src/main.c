#include "../tests/test_runner.c"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int
main(void)
{
    srand((unsigned) time(NULL));
    test_runner();
    return 0;
}
