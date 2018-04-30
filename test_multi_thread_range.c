#include "lsm.h"
#include "test.h"

int mainC()
{
    int run_size = 2;
    int level_ratio = 3;
    int level_size = 6;
    int thread_size = 3;

    lsm* tree = createLSMTree(run_size, level_size, level_ratio, thread_size);
	printf("Testing multi-threaded range scan");
    runTest(tree);
    return 0;
}