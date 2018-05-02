#ifndef LSM_H
#define LSM_H

#include <stdint.h>
#include <stdlib.h>
#include "hasht.h"
#include "fileutil.h"

#define DEFAULT_LSM_LEVEL_RATIO 2
#define DEFAULT_LSM_LEVEL 24
#define MAX_LSM_RUN_SIZE 10
#define DEFAULT_THREAD_SIZE 1
#define TRUE 1
#define FALSE 0
#define EMPTY_VALUE -0
#define SINGLE_THREAD 1
#define MULTI_THREAD 0
#define DEFAULT_FALSE_POSITIVE_RATE 0.05

typedef struct lsm_t {
	hashTable* l0;
	uint32_t max_level;
	uint32_t max_thread;
} lsm;

typedef enum lsm_level_e {LSM_L1 = 0, LSM_L2 = 1,LSM_L3 = 2,LSM_L4 = 3,LSM_L5 = 4} lsm_level;

typedef struct fencePtr_t {
	uint32_t curr_page_size;
	uint32_t max_page_size;
	runHeader *page;	
}fencePtr;

typedef struct result_set_t {
	int total_search;
	int total_found;
	node* results;
}result_set;

typedef struct search_arg_t {
    int range_from;
    int range_to;
	int id;
	uint32_t level;
	uint32_t page_num;
	run* disk_page;
	hashTable* result;
}search_arg;

//global fence pointer variable
fencePtr *g_lsm_fence_ptr;

lsm* createLSMTree(int bucket_size, int max_level, int level_ratio, int thread_size, double false_positive_rate);
void put(lsm *tree,int32_t key,int32_t val);
pair get(lsm *tree,int32_t key);
void erase(lsm *tree,int32_t key);
void flush_to_disk(lsm* tree, hashTable* table, uint32_t table_size) ;
runHeader merge_sort(node** buckets, int size, node** result);
void printLinkedList(node* nodes);
hashTable* range(lsm* tree, int from, int to);
hashTable* single_thread_range(lsm* tree, int from, int to);
hashTable* multi_thread_range(lsm* tree, int from, int to);
void run_tiered_compaction(lsm* tree);
void printTree(lsm* tree);
char convert_bucket_state_to_char(int int_state);
void add_to_table(lsm* tree,node* newNode);
void printBloomTable(lsm* tree);
void* search_level_for_range(void* arguments);
void* search_page_for_range(void* arguments);
pair slow_get(lsm* tree, int32_t key);
pair get(lsm* tree, int32_t key);
//void* deprecated_search_page_for_range(void* arguments)
#endif /* LSM_H */