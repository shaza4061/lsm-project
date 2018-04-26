#include "lsm.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void runRangeTest(lsm* tree)
{
    int num_tests = 20;
    int keys[] = { 4217, 12742, 19293, 24817, 11442, 23119, 3615, 9833, 18813, 27973, 10011, 13281, 18604,
	30591, 19357, 1136, 5155, 11773, 27967, 3589 };
    int values[] = { 18327, 12030, 21433, 16080, 30308, 22560, 28559, 24474, 14869, 29634, 32085, 12027, 3372,
	27284, 23123, 22642, 30732, 8360, 8450, 19541 };

    printf("Testing range scan from LSM tree.\n");
    printf("Inserting %d key-value pairs.\n", num_tests);
    for(int i = 0; i < num_tests; i += 1) {
	put(tree, keys[i], values[i]);
	printf("\t(%d -> %d) \n", keys[i], values[i]);
    }
	printBloomTable(tree);
		
    int from = 3500;
    int to = 3700;
	printf("Searching for key from %d to %d.\n", from, to);

    hashTable* result = range(tree, from, to);
    for(int key = from; key <= to; key++) {
	pair item = look(result, key);
	if(item.state != INVALID && item.state != UNKNOWN)
	    printf("%d ", item.value);
    }

    for(int i = 0; i < (int)result->size; i++) {
	if(result->bucket[i] != NULL) {
	    node* current = result->bucket[i];
	    node* head = current;
	    while((current = head) != NULL) {
		head = (node*)head->next;
		free(current);
	    }
	}
    }
    free(result);
}
int main()
{
    int run_size = 2;
    int level_ratio = 3;
    int level_size = 6;
	int thread_size = 4;

    lsm* tree = createLSMTree(run_size, level_size, level_ratio,thread_size);
    runRangeTest(tree);
    return 0;
}