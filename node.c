#include <stdint.h>
#include "node.h"

node *createNode(int32_t key,int32_t val, status currStat){
	node *newNode = (node*)malloc(sizeof(node));
	newNode->keyValue.key = key;
    newNode->keyValue.value = val;
	newNode->keyValue.state = currStat;
	newNode->next = NULL;
	
	return newNode;
}