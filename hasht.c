#include "hasht.h"
#include <stdlib.h>
#include <string.h>

hashTable* createTable(int32_t size)
{
    hashTable* t = (hashTable*)malloc(sizeof(hashTable));
    t->size = size;
    t->bucket = (node**)malloc(sizeof(node*) * size);
    int i;
    for(i = 0; i < size; i++)
	t->bucket[i] = NULL;
    return t;
}

void add(hashTable* t, node* newNode)
{
    int pos = hashCode(t, newNode->keyValue.key);
	if (t->bucket[pos] == NULL) {
		t->bucket[pos] = newNode;
	} else {
		node* list = t->bucket[pos];
		newNode->next = (struct node*)list;
		t->bucket[pos] = newNode;
	}  
}

uint32_t hashCode(hashTable* t, int32_t key)
{
    return abs(key) % t->size;
}

pair look(hashTable* t, int32_t key)
{
	pair result;
	result.state = UNKNOWN;
	
    int pos = hashCode(t, key);
    node* list = t->bucket[pos];
    node* temp = list;
    while(temp) {
	if(temp->keyValue.key == key) {
		result.key = temp->keyValue.key;
		result.value = temp->keyValue.value;
		result.state = temp->keyValue.state;
	    return result;
	}
	temp = (node*)temp->next;
    }
    return result;
}

void wipe(hashTable* t, node* delNode)
{
    add(t, delNode);
}