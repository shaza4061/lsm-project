#include "lsm.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SINGLE_THREAD 1
#define MULTI_THREAD 0

void runRangeTest(lsm* tree)
{
    int num_tests = 20;
    int keys[] = { 4217, 12742, 19293, 24817, 11442, 23119, 3615, 9833, 18813, 27973, 10011, 13281, 18604, 30591, 19357,
	1136, 5155, 11773, 27967, 3589 };
    int values[] = { 18327, 12030, 21433, 16080, 30308, 22560, 28559, 24474, 14869, 29634, 32085, 12027, 3372, 27284,
	23123, 22642, 30732, 8360, 8450, 19541 };

    printf("Testing range scan from LSM tree.\n");
    printf("Inserting %d key-value pairs.\n", num_tests);
    for(int i = 0; i < num_tests; i += 1) {
	put(tree, keys[i], values[i]);
	printf("\t(%d -> %d) \n", keys[i], values[i]);
    }

    int from = 3500;
    int to = 3700;
    printf("Searching for key from %d to %d.\n", from, to);

    hashTable* result;
	result = single_thread_range(tree, from, to);

    for(int key = from; key < to; key++) {
	pair item = look(result, key);
	if(item.state != INVALID && item.state != UNKNOWN) {
	    if(item.key == 3589 && item.value != 19541) {
		printf("FAIL: Expected [%d:%d], Actual [%d:%d]\n", key, keys[key], item.key, item.value);
		exit(1);
	    }
	    if(item.key == 3615 && item.value != 28559) {
		printf("FAIL: Expected [%d:%d], Actual [%d:%d]\n", key, keys[key], item.key, item.value);
		exit(1);
	    }
	}
    }

    for(int i = 0; i < (int)result->size; i++) {
		
	if(result->bucket[i] != NULL) {
	    node* head = result->bucket[i];
	    node* current = head;
	    while(head != NULL) {
		head = (node*)head->next;
		result->bucket[i] = head;
		current->next = NULL;
		free(current);
	    }
	}
    }
    printf("PASS\n");
    free(result);
}
int main()
{
    int run_size = 2;
    int level_ratio = 3;
    int level_size = 6;
    int thread_size = 4;
	double false_positive_rate = DEFAULT_FALSE_POSITIVE_RATE;

    lsm* tree = createLSMTree(run_size, level_size, level_ratio, thread_size,false_positive_rate);
	printf("Testing single-threaded range scan");
    runRangeTest(tree);
    return 0;
}