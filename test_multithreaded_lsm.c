#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lsm.h"
#include "test.h"

// This code is designed to test the correctness of your implementation.
// You do not need to significantly change it.
// Compile and run it in the command line by typing:
// make test; ./test

int mainB()
{
    int run_size = 2;
    int level_ratio = 3;
    int level_size = 6;
	int thread_size = DEFAULT_THREAD_SIZE;

    lsm* tree = createLSMTree(run_size, level_size, level_ratio,thread_size);
    runTest(tree);
    return 0;
}
