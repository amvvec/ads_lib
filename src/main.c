#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "test_runner.h"

int main(void)
{
    srand(time(NULL));
    test_runner();
    return 0;
}
