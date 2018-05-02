#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lsm.h"
#include "test.h"

// This code is designed to test the correctness of your implementation.
// You do not need to significantly change it.
// Compile and run it in the command line by typing:
// make test; ./test

int main()
{
    int run_size = 5;
    int level_ratio = 3;
    int level_size = 6;
	int thread_size = SINGLE_THREAD;
	double false_positive_rate = DEFAULT_FALSE_POSITIVE_RATE;

    lsm* tree = createLSMTree(run_size, level_size, level_ratio,thread_size,false_positive_rate);
    runTest(tree);
    return 0;
}
