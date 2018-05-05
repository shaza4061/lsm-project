#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <stdint.h>
#include <stdio.h>
#include "node.h"
#include "constant.h"
#include <string.h>
#define MAX_DATA_PATH 5

typedef struct runHeader_t {
	uint32_t pairCount;
	int32_t min;
	int32_t max;
} runHeader;

typedef struct run_t {
	runHeader header;
	pair* keyValue;
}run;

typedef struct level_data_path_t {
	int from_level;
	int to_level;
	char path[50];
}level_data_path;



void getFileName(uint32_t level_n,char** filename);
void read_a_page(char *filename,uint32_t pageNum, uint32_t run_size,run** page);
void merge_level(int fromLevel, int toLevel, uint32_t run_size);
int append_to_file(char* filename,runHeader header, node* list);
void data_path_add(int from_level, int to_level, char path[]);
char* data_path_get(int level);
void data_path_init();
void initFencePtr(int level);

#endif /* FILEUTIL_H */