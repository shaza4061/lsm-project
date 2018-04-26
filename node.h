#ifndef NODE_H
#define NODE_H

#include <stdlib.h>
#include <stdint.h>

typedef enum status_e { VALID = 1, INVALID =0, UNKNOWN=2 }status;
typedef struct pair_t {
	int32_t key;
	int32_t value;
	status state;
}pair;

typedef struct node_t
{
    pair keyValue;	
    struct node* next;
}
node;

typedef struct linked_list_t
{
    int value;	
    struct linked_list* next;
}
linked_list;

node *createNode(int32_t key,int32_t val, status currStat);

#endif /* NODE_H */