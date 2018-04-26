#ifndef HASHT_H
#define HASHT_H

#include "node.h"
#include <stdint.h>
#define DEFAULT_BUCKET_SIZE 2
//#define MAX_CHAIN 10
//#define DEFAULT_LOAD_FACTOR 0.75
//#define HASHTABLE_HIGH_WATERMARK BUCKET_SIZE*MAX_CHAIN*LOAD_FACTOR

typedef struct hashTable_t {
    uint32_t currSize;
    uint32_t size;
    node** bucket;
} hashTable;

hashTable* createTable(int32_t size);
uint32_t hashCode(hashTable* t, int32_t key);
void add(hashTable* t, node* newNode);
pair look(hashTable* t, int32_t key);
void wipe(hashTable* t, node* newNode);
//node* createNode(int32_t key, int32_t val, status currStat);

#endif /* HASHT_H */