#include "lsm.h"
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

    while((c = getopt(argc, argv, "sb:l:r:t:")) != -1)
	switch(c) {
	case 'b':
	    run_size = atoi(optarg);
		if (run_size > MAX_LSM_RUN_SIZE) {
			printf("ERROR: Run size cannot be bigger than %d. Terminating...", MAX_LSM_RUN_SIZE);
			exit(1);
		}
	    break;
	case 'l':
	    level_size = atoi(optarg);
		if (level_size%2 != 0) {
			printf("ERROR: Level size must be an even number. Terminating...");
			exit(1);
		}
	    break;
	case 'r':
	    level_ratio = atoi(optarg);
	    break;
	case 't':
	    thread_size = atoi(optarg);
		if (thread_size%2 != 0) {
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

    tree = createLSMTree(run_size, level_size, level_ratio);

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
	}
	fclose(read_ptr);
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