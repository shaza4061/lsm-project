#include "bloom.h"
#include "constant.h"
#include "lsm.h"
#include "node.h"
#include <inttypes.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

lsm* createLSMTree(int bucket_size, int max_level, int level_ratio, int thread_size, double false_positive_rate)
{
    lsm* tree = (lsm*)malloc(sizeof(lsm));
    tree->l0 = createTable(bucket_size);
    tree->max_level = max_level;
    tree->max_thread = thread_size;
    tree->l0->currSize = 0;
    g_lsm_fence_ptr = malloc(max_level * sizeof(fencePtr));
    g_bloom_filter_ptr = malloc(max_level * sizeof(bloom_filter));
	
    for(int i = 0; i < max_level; i++) {
	int max_page_size = bucket_size * level_ratio * ((int)pow((double)level_ratio, i));
	int filter_size = ceil((bucket_size * log(false_positive_rate)) / log(1 / pow(2, log(2))));
	
	g_lsm_fence_ptr[i].curr_page_size = 0;
	g_lsm_fence_ptr[i].max_page_size = max_page_size;
	g_lsm_fence_ptr[i].page = malloc(max_page_size * (sizeof(runHeader) + 2));
	g_bloom_filter_ptr[i].page = malloc(max_page_size * (sizeof(bloom_filter_bitmap) + 2));
	g_bloom_filter_ptr[i].filter_size = filter_size;

	for(int j = 0; j < max_page_size; j++) {
	    g_lsm_fence_ptr[i].page[j].pairCount = 0;
	    g_lsm_fence_ptr[i].page[j].min = 0;
	    g_lsm_fence_ptr[i].page[j].max = 0;
		g_bloom_filter_ptr[i].page[j].bitmap = calloc(filter_size/8,sizeof(bloom_type));
	}
    }
    return tree;
}

result_set* createResultSet(uint32_t total_items)
{
    result_set* result = (result_set*)malloc(sizeof(result_set));
    result->total_search = total_items;
    result->total_found = 0;
    return result;
}

pair get(lsm* tree, int32_t key)
{
    int32_t from = key;
    int32_t to = key + 1;
    hashTable* result = range(tree, from, to);

    pair item = look(result, key);

    // cleanup
    for(int i = 0; i < (int)result->size; i++) {
	if(result->bucket[i] != NULL) {
	    node* current = result->bucket[i];
		node* head = NULL;
		do{
			head = current;
			head = (node*)head->next;
			free(current);
			current = head;
		}while(current!=NULL);
	}
    }

    free(result);
    return item;
}

pair slow_get(lsm* tree, int32_t key)
{
    pair value = look(tree->l0, key);
    if(value.state == UNKNOWN) {
	// search in disk
	for(uint32_t level_n = 0; level_n < tree->max_level; level_n++) { // iterate each level
	    if(g_lsm_fence_ptr[level_n].curr_page_size != 0) {            // if level contains data
		for(int page_num = g_lsm_fence_ptr[level_n].curr_page_size - 1; page_num >= 0;
		    page_num--) { // iterate run from the back
		    if(key <= g_lsm_fence_ptr[level_n].page[page_num].max &&
		        key >= g_lsm_fence_ptr[level_n].page[page_num].min) {
			// if key is in between the page then get the page
			if(contains(level_n, page_num, key) == TRUE) {
			    char* filename = getFileName(level_n);
			    run* page = read_a_page(filename, page_num, tree->l0->size);
			    for(uint32_t i = 0; i < page->header.pairCount; i++) {
				if(key == page->keyValue[i].key) {
				    value.key = page->keyValue[i].key;
				    value.value = page->keyValue[i].value;
				    value.state = page->keyValue[i].state;
				    return value;
				}
			    }
			}
		    }
		}
	    }
	}
    }
    return value;
}

void put(lsm* tree, int32_t key, int32_t value)
{
    node* newNode = createNode(key, value, VALID);
    add_to_table(tree, newNode);
}

void add_to_table(lsm* tree, node* newNode)
{
    add(tree->l0, newNode);
    tree->l0->currSize++;
    if(tree->l0->currSize == tree->l0->size) {
	flush_to_disk(tree, tree->l0, tree->l0->size);
	hashTable** tmpTable = (hashTable**)tree->l0->bucket;
	// reset hashtable
	for(uint32_t i = 0; i < tree->l0->size; i++) {
	    if(tree->l0->bucket[i] != NULL) {
		node* current = tree->l0->bucket[i];
		node* head = current;
		while((current = head) != NULL) {
		    head = (node*)head->next;
		    free(current);
		}
	    }
	}
	free(tmpTable);
	tree->l0 = createTable(tree->l0->size);
	tree->l0->currSize = 0;
    }
}

void erase(lsm* tree, int32_t key)
{
    int dummyInt = EMPTY_VALUE;
    node* n = createNode(key, dummyInt, INVALID);
    // wipe(tree->l0, n);
    add_to_table(tree, n);
}

void run_tiered_compaction(lsm* tree)
{
    if(g_lsm_fence_ptr[tree->max_level - 1].curr_page_size == g_lsm_fence_ptr[tree->max_level - 1].max_page_size) {
	printf(ERROR_NO_SPACE_AVAILABLE);
	exit(EXIT_FAILURE);
    }
    for(uint32_t i = 0; i < tree->max_level; i++) {
	if(g_lsm_fence_ptr[i].curr_page_size == g_lsm_fence_ptr[i].max_page_size) {
	    merge_level(i, i + 1, tree->l0->size);
	}
    }
}

void flush_to_disk(lsm* tree, hashTable* table, uint32_t table_size)
{
    // int level_n = s1;
    node* sortedList = NULL;
    runHeader header = merge_sort(table->bucket, table_size, &sortedList);
    if(g_lsm_fence_ptr[LSM_L1].curr_page_size == g_lsm_fence_ptr[LSM_L1].max_page_size) {
	run_tiered_compaction(tree);
    }

    char* filename = getFileName(LSM_L1);
    append_to_file(filename, header, sortedList);
    g_lsm_fence_ptr[LSM_L1].curr_page_size++;
    g_lsm_fence_ptr[LSM_L1].page[g_lsm_fence_ptr[LSM_L1].curr_page_size - 1].pairCount = header.pairCount;
    g_lsm_fence_ptr[LSM_L1].page[g_lsm_fence_ptr[LSM_L1].curr_page_size - 1].min = header.min;
    g_lsm_fence_ptr[LSM_L1].page[g_lsm_fence_ptr[LSM_L1].curr_page_size - 1].max = header.max;
}

runHeader merge_sort(node** buckets, int size, node** result)
{
    node* sortedNodes = NULL;
    node* head = NULL;
    node* tail = NULL;
    runHeader header;
    int count = 0;

    for(int i = 0; i < size; i++) {
	if(buckets[i] != NULL) {
	    node* list = buckets[i];
	    while(list) {
		node* newNode = createNode(list->keyValue.key, list->keyValue.value, list->keyValue.state);
		if(sortedNodes == NULL) {
		    // first entry
		    sortedNodes = newNode;
		    head = newNode;
		    tail = newNode;
		    count++;
		} else {
		    node* cursor = head;
		    if(newNode->keyValue.key < head->keyValue.key) {
			// node* temp = head;
			newNode->next = (struct node*)head;
			head = newNode;
			count++;
		    } else if(newNode->keyValue.key > tail->keyValue.key) {
			tail->next = (struct node*)newNode;
			tail = newNode;
			count++;
		    } else {
			// node is between head and tail
			while(cursor) {
			    node* next_cursor = (node*)cursor->next;
			    if(newNode->keyValue.key > cursor->keyValue.key &&
			        newNode->keyValue.key < next_cursor->keyValue.key) {
				node* temp = (node*)cursor->next;
				cursor->next = (struct node*)newNode;
				newNode->next = (struct node*)temp;
				count++;
				break;
			    }
			    cursor = (node*)cursor->next;
			}
		    }
		}
		list = (node*)list->next;
	    }
	}
    }
    *result = head;
    header.pairCount = count;
    header.min = head->keyValue.key;
    header.max = tail->keyValue.key;

    return header;
}

void printLinkedList(node* nodes)
{
    node* cursor = nodes;
    while(cursor) {
	printf("%d:%d ", cursor->keyValue.key, cursor->keyValue.value);
	cursor = (node*)nodes->next;
    }
}

hashTable* range(lsm* tree, int from, int to) {
	if(tree->max_thread==SINGLE_THREAD) {
		return single_thread_range(tree,from,to);
	} else {
		return multi_thread_range(tree,from,to);
	}
}

hashTable* single_thread_range(lsm* tree, int from, int to)
{
    int total_search_items = to - from;
    int total_found_items = 0;
    int level_size = tree->max_level;
    // init hashtable to store result
    hashTable* result = createTable(total_search_items);
    // Search in memory first
    for(int32_t key = from; key < to; key++) {
	pair value = look(tree->l0, key);
	// add to result table if found
	if(value.state != UNKNOWN) {
	    node* newNode = createNode(value.key, value.value, value.state);
	    add(result, newNode);
	    total_found_items++;
	}
    }

    // if there's still item needs to be found
    // Create thread to search for candidate block
    // at each level
    if(total_found_items != total_search_items) {
	linked_list** page_num_list = calloc(level_size, sizeof(linked_list*) + 2);

	for(int current_level = 0; current_level < level_size; current_level++) {

	    void* temp = NULL;
	    search_arg* fence_ptr_search_arg = malloc(sizeof(search_arg));
	    fence_ptr_search_arg->level = current_level;
	    fence_ptr_search_arg->range_from = from;
	    fence_ptr_search_arg->range_to = to;

	    // Search the fence pointer
	    temp = search_level_for_range(fence_ptr_search_arg);
	    page_num_list[current_level] = (linked_list*)temp;
	    linked_list* page = page_num_list[current_level];

	    // if found pages with possible values and number of found items is lesser than expected
	    while(page != NULL && (total_found_items < total_search_items)) {
		// Search the disk based on possible page location
		char* filename = getFileName(current_level);
		run* disk_page = read_a_page(filename, page->value, tree->l0->size);

		search_arg* page_search_arg = malloc(sizeof(search_arg));
		page_search_arg->range_from = from;
		page_search_arg->range_to = to;
		page_search_arg->level = current_level;
		page_search_arg->disk_page = disk_page;
		page_search_arg->page_num = page->value;
		page_search_arg->result = result;

		search_page_for_range(page_search_arg);
		page = (linked_list*)page->next;
	    }
	}
    }

    return result;
}

hashTable* multi_thread_range(lsm* tree, int from, int to)
{
    int level_size = tree->max_level;
    int total_search_items = to - from;
    int total_found_items = 0;
    int max_thread_count = (int)tree->max_thread;
    // init hashtable to store result
    hashTable* result = createTable(total_search_items);
    // Search in memory first
    for(int32_t key = from; key < to; key++) {
	pair value = look(tree->l0, key);
	// add to result table if found
	if(value.state != UNKNOWN) {
	    node* newNode = createNode(value.key, value.value, value.state);
	    add(result, newNode);
	    total_found_items++;
	}
    }

    // if there's still item needs to be found

    if(total_found_items != total_search_items) {
	pthread_t* search_threads = malloc(sizeof(pthread_t) * tree->max_thread);
	linked_list** page_num_list = calloc(level_size, sizeof(linked_list*) + 2);
	int thread_still_running = FALSE;
	int scanning_in_progress = TRUE;
	int current_level = 0;
	int thread_count = 0;
	
	//Create a lookup table to store thread information
	int *thread_lookup_table = malloc(max_thread_count * sizeof(int *)); 
	
	// Create thread to search for candidate block
	// at each level
	while(thread_still_running || scanning_in_progress) {
	    // if all thread is used, wait for them to complete
	    if(thread_count == max_thread_count || !scanning_in_progress) {
		for(int thread_num = 0; thread_num < thread_count; thread_num++) {
		    void* temp = NULL;
		    pthread_join(search_threads[thread_num], &temp);
			int level = thread_lookup_table[thread_num];
		    page_num_list[level] = (linked_list*)temp;
		}
		// reset thread count
		thread_count = 0;
		thread_still_running = FALSE;
	    }
	    if(current_level < level_size) {
		search_arg* arg = malloc(sizeof(search_arg));
		arg->level = current_level;
		arg->id = thread_count;
		arg->range_from = from;
		arg->range_to = to;
		
		//register thread in thread lookup table
		thread_lookup_table[thread_count] = current_level;
		if(pthread_create(&search_threads[thread_count], NULL, search_level_for_range, arg) != 0) {
		    printf("%s", ERROR_CREATING_THREAD);
		}
		current_level++;
		thread_count++;
		thread_still_running = TRUE;
	    } else {
		scanning_in_progress = FALSE;
	    }
	}

	//For each level, we retrieve the potential page and perform search
	for(int level = 0; level < level_size; level++) {
	    char* filename = getFileName(level);
	    linked_list* page = page_num_list[level];
	    while(page != NULL) {
		run* disk_page = read_a_page(filename, page->value, tree->l0->size);

		// split the range and search from page
		// calculate number of loop to complete the search
		int increment = total_search_items / max_thread_count;
		int from_key = from;
		int to_key = 0;
		int total_loop = increment;
		if(increment == 0)
		    to_key = to;
		else
		    to_key = from_key + increment;

		// add 1 for odd number of items
		if(total_search_items % max_thread_count != 0) {
		    total_loop++;
		}

		for(int count = 0; count < max_thread_count; count++) {
		    search_arg* arg = malloc(sizeof(search_arg));
		    arg->range_from = from_key;
		    arg->range_to = to_key;
		    arg->id = count;
		    arg->level = level;
		    arg->disk_page = disk_page;
		    arg->page_num = page->value;
		    arg->result = result;

		    if(pthread_create(&search_threads[count], NULL, search_page_for_range, arg) != 0) {
			printf("%s", ERROR_CREATING_THREAD);
		    }
		    from_key = to_key + 1;
		    int temp = from_key + increment;
		    if(temp > to)
			to_key = to;
		    else
			to_key += increment;
		}

		// Join back the thread
		for(int count = 0; count < max_thread_count; count++) {
		    pthread_join(search_threads[count], NULL);
		}
		page = (linked_list*)page->next;
	    }
	}

	// release page_num_list memory
	for(int k = 0; k < level_size; k++) {
	    linked_list* head = page_num_list[k];
	    while(head != NULL) {
		linked_list* temp = (linked_list*)head;
		head = (linked_list*)head->next;
		free(temp);
	    }
	}
	free(search_threads);
	free(page_num_list);
    }
    return result;
}

void* search_page_for_range(void* arguments)
{
    search_arg* args = (search_arg*)arguments;
    int from = args->range_from;
    int to = args->range_to;
    int level = args->level;
    int page_num = args->page_num;
    run* page = args->disk_page;
    hashTable* result = args->result;
    node* result_nodes = NULL;

    for(uint32_t i = 0; i < page->header.pairCount; i++) {
	for(int key = from; key <= to; key++) {
	    // printf("Thread#%d: Searching for key %d\n",id,key);
	    pair item = look(result, key);
	    // if key is not in current result and match with bloom filter
	    if(item.state == UNKNOWN && contains(level, page_num, key) == TRUE) {
		if(key == page->keyValue[i].key) {
		    node* newNode = createNode(page->keyValue[i].key, page->keyValue[i].value, page->keyValue[i].state);
		    if(result_nodes == NULL) {
			result_nodes = newNode;
		    } else {
			result_nodes->next = (struct node*)newNode;
			result_nodes = (node*)result_nodes->next;
		    }
		    add(result, newNode);
		}
	    }
	}
    }
    args->result = NULL;
    args->disk_page = NULL;
    free(args);
    return (void*)NULL;
}

void* search_level_for_range(void* arguments)
{
    search_arg* args = (search_arg*)arguments;
    uint32_t level = args->level;
    int from = args->range_from;
    int to = args->range_to;
    free(args);

    int max_page_num = g_lsm_fence_ptr[level].curr_page_size - 1;
    linked_list* cursor = NULL;
    linked_list* list = NULL;

    for(int highest_page_num = max_page_num; highest_page_num >= 0; highest_page_num--) {
	for(int key = from; key < to; key++) {
	    if(key >= g_lsm_fence_ptr[level].page[highest_page_num].min &&
	        key <= g_lsm_fence_ptr[level].page[highest_page_num].max) {
		if(contains(level, highest_page_num, key) == TRUE) {
		    linked_list* newList = (linked_list*)malloc(sizeof(linked_list));
		    newList->value = highest_page_num;
		    newList->next = NULL;
		    if(list == NULL) {
			list = newList;
			cursor = list;
		    } else {
			cursor->next = (struct linked_list*)newList;
			cursor = (linked_list*)cursor->next;
		    }
		    // exit loop;
		    // we don't need to check if all the range is in the page since
		    // we will read the entire page from the disk
		    break;
		}
	    }
	}
    }
    return (void*)list;
}

void printTree(lsm* tree)
{
    printf("Level 0:");
    int l0_size = (int)tree->l0->size;
    for(int i = 0; i < l0_size; i++) {
	printf("\n\tBucket %d:", i);
	node* ptr = tree->l0->bucket[i];
	while(ptr) {
	    printf("[%d:%d:V/I=%c]\t", ptr->keyValue.key, ptr->keyValue.value,
	        convert_bucket_state_to_char(ptr->keyValue.state));
	    ptr = (node*)ptr->next;
	}
	printf("\n");
    }

    for(uint32_t j = 0; j < tree->max_level; j++) {
	printf("Level %d:\n", j + 1);
	char* filename = getFileName(j);
	for(uint32_t k = 0; k < g_lsm_fence_ptr[j].curr_page_size; k++) {
	    run* page = read_a_page(filename, k, tree->l0->size);
	    printf("\tPage %d: ", k);
	    for(uint32_t l = 0; l < page->header.pairCount; l++) {
		printf("[%d:%d:V/I=%c]\t", page->keyValue[l].key, page->keyValue[l].value,
		    convert_bucket_state_to_char((int)page->keyValue[l].state));
	    }
	    printf("\n");
	}
	printf("\n");
    }
    for(uint32_t level = 0; level < tree->max_level; level++) {
	printf("Fence pointer for Level [%d] :\n", level + 1);
	printf("\t current page size : %d\n", g_lsm_fence_ptr[level].curr_page_size);
	for(uint32_t fence = 0; fence < g_lsm_fence_ptr[level].curr_page_size; fence++) {
	    printf("\t page :%d\n", fence);
	    printf("\t count : %d\n", g_lsm_fence_ptr[level].page[fence].pairCount);
	    printf("\t Min: %d\n", g_lsm_fence_ptr[level].page[fence].min);
	    printf("\t Max: %d\n", g_lsm_fence_ptr[level].page[fence].max);
	}
    }
}

char convert_bucket_state_to_char(int int_state)
{
    char bucket_status;
    if(int_state == TRUE)
	bucket_status = 'V';
    else
	bucket_status = 'I';
    return bucket_status;
}

//void printBloomTable(lsm* tree)
//{
//    for(int i = 0; i < (int)tree->max_level; i++) {
//	for(int j = 0; j < (int)g_lsm_fence_ptr[i].curr_page_size; j++) {
//	    printf("bloom[%d][%d] = %" PRIu32 "\n", i, j, g_bloom_table[i][j]);
//	}
//    }
//}