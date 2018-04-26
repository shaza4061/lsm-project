#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <stdint.h>
#include <stdio.h>
#include "node.h"
#include "constant.h"
#include <string.h>

typedef struct runHeader_t {
	uint32_t pairCount;
	int32_t min;
	int32_t max;
} runHeader;

typedef struct run_t {
	runHeader header;
	pair* keyValue;
}run;

char * getFileName(uint32_t level_n);
run *read_a_page(char *filename,uint32_t pageNum, uint32_t run_size);
void merge_level(int fromLevel, int toLevel, uint32_t run_size);
int append_to_file(char* filename,runHeader header, node* list);

#endif /* FILEUTIL_H */