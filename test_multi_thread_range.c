#include "lsm.h"
#include "test.h"

int main()
{
    int run_size = 2;
    int level_ratio = 3;
    int level_size = 6;
    int thread_size = 3;
	double false_positive_rate = DEFAULT_FALSE_POSITIVE_RATE;

    lsm* tree = createLSMTree(run_size, level_size, level_ratio, thread_size,false_positive_rate);
	printf("Testing multi-threaded range scan");
    runTest(tree);
    return 0;
}