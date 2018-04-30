#include "lsm.h"
#include "fileutil.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define COLS 30
#define WIDTH 5

void parse_command(char[]);
lsm* tree = NULL;
int silent_mode = 0;
int main(int argc, char** argv)
{
    int run_size = DEFAULT_BUCKET_SIZE;
    int level_ratio = DEFAULT_LSM_LEVEL_RATIO;
    int level_size = DEFAULT_LSM_LEVEL;
    int thread_size = DEFAULT_THREAD_SIZE;
    int c;
    opterr = 0;

    while((c = getopt(argc, argv, "sb:l:r:t:d:")) != -1)
	switch(c) {
	case 'd': {
		data_path_init();
	    char file_path[5][100];
	    int row = 0;
	    char* path_config = strtok(optarg, ",");

	    while(path_config != NULL) {
		strcpy(file_path[row], path_config);
		row++;
		path_config = strtok(NULL, "-");
		
		if(row >= MAX_DATA_PATH) {
		    printf("Number of  path define cannot exceed %d\n", MAX_DATA_PATH);
		    exit(EXIT_FAILURE);
		}
	    }

	    for(int i = 0; i < row; i++) {
		char* level_config = strtok(file_path[i], "=");
		while(level_config != NULL) {
		    char code = level_config[0];
		    int length = strlen(level_config);
		    if(code == 'r') {
			char strFromLevel[5] = "";
			char strToLevel[5] = "";
			char path[50] = "";
			char temp1[50]="";
			int intFromLevel, intToLevel,strLength;
			strLength = strlen(level_config);
			strncpy(temp1, level_config+1, strLength);
//			temp1[strlen(level_config)] = '\0';
			level_config = strtok(NULL, "=");
			strncpy(path,level_config,strlen(level_config));
			const char* ptr = strchr(temp1, '-');
			strncpy(strFromLevel, temp1, ptr - temp1);
			strncpy(strToLevel, ptr + 1, strlen(temp1) - (ptr - temp1 + 1));
			intFromLevel = atoi(strFromLevel);
			intToLevel = atoi(strToLevel);
			data_path_add(intFromLevel,intToLevel,path);
			break;

		    } else if(code == '+') {
			char strLevel[5] = "";
			char path[50] = "";
			int intLevel;
			strncpy(strLevel, level_config + 1, length - 1);
//			strLevel[length] = '\0';
			intLevel = atoi(strLevel);
			level_config = strtok(NULL, "=");
			strncpy(path, level_config, strlen(level_config));
			data_path_add(intLevel,100,path);
			break;
		    } else if(code == 's') {
			char strLevel[5] = "";
			char path[50] = "";
			int intLevel;
			strncpy(strLevel, level_config + 1, length - 1);
			strLevel[length] = '\0';
			intLevel = atoi(strLevel);
			level_config = strtok(NULL, "=");
			strncpy(path, level_config, strlen(level_config));
			data_path_add(intLevel,intLevel,path);
			printf("Level %d data is saved at %s\n", intLevel, path);
			break;
		    } else {
			printf("Invalid operator %c. Expected 'r','s', or '+'\n", code);
			exit(EXIT_FAILURE);
		    }
		}
	    }
	    break;
	}

	case 'b': {
	    run_size = atoi(optarg);
	    if(run_size > MAX_LSM_RUN_SIZE) {
		printf("ERROR: Run size cannot be bigger than %d. Terminating...", MAX_LSM_RUN_SIZE);
		exit(1);
	    }
	    break;
	}
	case 'l':
	    level_size = atoi(optarg);
	    if(level_size % 2 != 0) {
		printf("ERROR: Level size must be an even number. Terminating...");
		exit(1);
	    }
	    break;
	case 'r':
	    level_ratio = atoi(optarg);
	    break;
	case 't':
	    thread_size = atoi(optarg);
	    if(thread_size % 2 != 0) {
		printf("ERROR: Thread size must be an even number. Terminating...");
		exit(1);
	    }
	    break;
	case 's':
	    silent_mode = 1;
	    break;
	case '?':
	    if(isprint(optopt))
		fprintf(stderr, "Unknown option `-%c'.\n", optopt);
	    else
		fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
	    return 1;
	default:
	    abort();
	}
    if(!silent_mode) {
	printf("**************************\n");
	printf("Bucket size:%d\n", run_size);
	printf("Tree height:%d\n", level_size);
	printf("Ratio size:%d\n", level_ratio);
	printf("Thread count:%d\n", thread_size);
	printf("**************************\n");
    }

    tree = createLSMTree(run_size, level_size, level_ratio, thread_size);

    char command[50] = "";
    int chr;

    do {
	int cols;
	for(cols = 0; (chr = getchar()) != '\n'; cols++) {
	    command[cols] = chr;
	}
	command[cols] = '\0';
	//	printf("Command :%s\n", command);
	parse_command(command);
    } while(strcmp(command, "q"));

    return 0;
}

void parse_command(char command[])
{
    char* tokens[100];
    size_t n = 0;

    for(char* p = strtok(command, " "); p; p = strtok(NULL, " ")) {
	if(n >= 50) {
	    // maximum number of storable tokens exceeded
	    break;
	}
	tokens[n++] = p;
    }
    char op = tokens[0][0];
    switch(op) {
    case 'p': {
	put(tree, atoi(tokens[1]), atoi(tokens[2]));
	break;
    }
    case 'g': {
	pair value = get(tree, atoi(tokens[1]));
	if(value.state != INVALID && value.state != UNKNOWN)
	    if(!silent_mode) {
		printf("%d", value.value);
		printf("\n");
	    }

	break;
    }
    case 'r': {
	int from = atoi(tokens[1]);
	int to = atoi(tokens[2]);

	hashTable* result = range(tree, from, to);
	for(int key = from; key < to; key++) {
	    pair item = look(result, key);
	    if(item.state != INVALID && item.state != UNKNOWN)
		if(!silent_mode) {
		    printf("%d:%d", item.key, item.value);
		    printf("\n");
		}
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
	break;
    }
    case 'd': {
	erase(tree, atoi(tokens[1]));
	break;
    }
    case 'l': {
	int count = 0;
	printf("load %s\n", tokens[1]);
	FILE* read_ptr = fopen(tokens[1], READ_BINARY);
	if(read_ptr == NULL) {
	    fprintf(stderr, ERROR_OPENING_FILE_FOR_READING, tokens[1]);
	    exit(EXIT_FAILURE);
	}
	while(!feof(read_ptr)) {
	    int key, value;
	    fread(&key, sizeof(int), 1, read_ptr);
	    fread(&value, sizeof(int), 1, read_ptr);
	    put(tree, key, value);
	    count++;
	}
	fclose(read_ptr);
	printf("%d keys loaded\n", count);
	break;
    }
    case 's': {
	printTree(tree);
	break;
    }
    case 'q': {
	if(!silent_mode)
	    printf("Bye...\n");
	exit(EXIT_SUCCESS);
	break;
    }
    default:
	printf("Unknown command:%c\n", command[0]);
    }
}