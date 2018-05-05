#include "bloom.h"
#include "fileutil.h"
#include "hasht.h"
#include "lsm.h"
#include <string.h>

char* itoa(int, char*, int);
static int data_path_row_count;
static level_data_path g_data_path_lookup[MAX_DATA_PATH];

void getFileName(uint32_t level_n,char** filename)
{
    char level[2];
    char* path  = data_path_get(level_n+1);

    snprintf(level, 2, "%d", level_n);
	if (path != NULL) { // if path exist
		*filename = calloc(1,strlen(path) + strlen(LSM_FILENAME_PREFIX) + strlen(level) + strlen(FILE_EXTENSION) + 1);
		strncpy(*filename,path,strlen(path));
	} else {
		*filename = calloc(1,strlen(LSM_FILENAME_PREFIX) + strlen(level) + strlen(FILE_EXTENSION) + 1);
		strncpy(*filename,"",1);
	}
		
    strncat(*filename, LSM_FILENAME_PREFIX,strlen(LSM_FILENAME_PREFIX));
    strncat(*filename, level,strlen(level));
    strncat(*filename, FILE_EXTENSION,strlen(FILE_EXTENSION));
}
void read_a_page(char* filename, uint32_t pageNum, uint32_t run_size,run** page)
{
    FILE* read_ptr;
    long offset = (sizeof(runHeader) + sizeof(pair) * run_size) * pageNum;

    *page = (run*)malloc(sizeof(run));
    (*page)->keyValue = calloc(run_size, sizeof(*((*page)->keyValue)));

    read_ptr = fopen(filename, READ_BINARY);
    if(read_ptr == NULL) {
	fprintf(stderr, ERROR_OPENING_FILE_FOR_READING, filename);
	exit(EXIT_FAILURE);
    }

    fseek(read_ptr, offset, SEEK_SET);
    fread(&((*page)->header), sizeof(runHeader), 1, read_ptr);
    fread((*page)->keyValue, sizeof(pair), (*page)->header.pairCount, read_ptr);

    if(fclose(read_ptr)) {
	printf("error closing file.");
	exit(EXIT_FAILURE);
    }
//    return page;
}

void initFencePtr(int level) {
	uint32_t max_page_size = g_lsm_fence_ptr[level].max_page_size;
	g_lsm_fence_ptr[level].page = malloc(max_page_size * (sizeof(runHeader) + 2));
	for(int j = 0; j < (int) max_page_size; j++) {
	    g_lsm_fence_ptr[level].page[j].pairCount = 0;
	    g_lsm_fence_ptr[level].page[j].min = 0;
	    g_lsm_fence_ptr[level].page[j].max = 0;
	}
}

void merge_level(int fromLevel, int toLevel, uint32_t run_size)
{
    char* fromFileName = NULL;
	char* toFileName = NULL;
	getFileName(fromLevel,&fromFileName);    
	getFileName(toLevel,&toFileName);

    FILE* write_ptr = fopen(toFileName, APPEND_BINARY);
	if (g_lsm_fence_ptr[toLevel].page == NULL) {
		//initialize fence pointer if empty
		initFencePtr(toLevel);
	}
	
    if(write_ptr == NULL) {
	fprintf(stderr, ERROR_OPENING_FILE_FOR_READING, toFileName);
	exit(EXIT_FAILURE);
    }

    for(uint32_t i = 0; i < g_lsm_fence_ptr[fromLevel].curr_page_size; i++) {	
	run* page = NULL;
	read_a_page(fromFileName, i, run_size,&page);
	fwrite(&page->header, sizeof(runHeader), 1, write_ptr);
	fwrite(page->keyValue, sizeof(pair), page->header.pairCount, write_ptr);

	g_lsm_fence_ptr[toLevel].curr_page_size++;
	g_lsm_fence_ptr[toLevel].page[g_lsm_fence_ptr[toLevel].curr_page_size - 1].pairCount = page->header.pairCount;
	g_lsm_fence_ptr[toLevel].page[g_lsm_fence_ptr[toLevel].curr_page_size - 1].min = page->header.min;
	g_lsm_fence_ptr[toLevel].page[g_lsm_fence_ptr[toLevel].curr_page_size - 1].max = page->header.max;
	moveBloomFilter(fromLevel, i, toLevel, g_lsm_fence_ptr[toLevel].curr_page_size - 1);
	
	free(page);
	page = NULL;
    }

    if(fclose(write_ptr)) {
	printf("error closing file.");
	exit(EXIT_FAILURE);
    }

    remove(fromFileName);
	free(fromFileName);
	free(toFileName);
	fromFileName = NULL;
	toFileName = NULL;
    // reset fence pointer of merged level
    g_lsm_fence_ptr[fromLevel].curr_page_size = 0;
//    reset_bloom_table_row(fromLevel, g_lsm_fence_ptr[fromLevel].max_page_size);
}

int append_to_file(char* filename, runHeader header, node* list)
{
    FILE* write_ptr;
    write_ptr = fopen(filename, APPEND_BINARY);
    int result = FALSE;

    if(write_ptr == NULL) {
	fprintf(stderr, ERROR_OPENING_FILE_FOR_WRITING, filename);
	return FALSE;
    }

    if(list != NULL) {
	// write meta-data at the beginning of run
	node* cursor = list;
	fwrite(&header, sizeof(runHeader), 1, write_ptr);
	while(cursor) {
	    fwrite(&cursor->keyValue, sizeof(pair), 1, write_ptr);
	    addToBloom(LSM_L1, g_lsm_fence_ptr[LSM_L1].curr_page_size, cursor->keyValue.key);
	    cursor = (node*)cursor->next;
	}

	if(fclose(write_ptr)) {
	    printf("error closing file.");
	    exit(EXIT_FAILURE);
	}
	result = TRUE;
    }
    return result;
}

void data_path_add(int from_level, int to_level, char path[])
{
    g_data_path_lookup[data_path_row_count].from_level = from_level;
    g_data_path_lookup[data_path_row_count].to_level = to_level;
    strncpy(g_data_path_lookup[data_path_row_count].path, path, strlen(path));
	data_path_row_count++;
}

char* data_path_get(int level)
{

    for(int i = 0; i < data_path_row_count; i++) {
	if(level >= g_data_path_lookup[i].from_level && level <= g_data_path_lookup[i].to_level) {
	    return g_data_path_lookup[i].path;
	    break;
	}
    }
    return NULL;
}

void data_path_init()
{
    data_path_row_count = 0;
    for(int row = 0; row < MAX_DATA_PATH; row++) {
	g_data_path_lookup[row].from_level = 0;
	g_data_path_lookup[row].to_level = 0;
	memset(&g_data_path_lookup[row].path, 0, sizeof(g_data_path_lookup[row].path));
    }
}