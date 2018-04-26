#ifndef BLOOM_H
#define BLOOM_H

#include <stdint.h>
#define M 64   // Number of bits in the Bloom filter
//Bloom filter configuration
//n = 10
//p = 0.05 (1 in 20)
//m = 64 (8B)
//k = 3



typedef uint32_t bloom_type;

void addToBloom(uint32_t level, uint32_t page_num, uint32_t key);
int contains(uint32_t level, uint32_t page_num,uint32_t key);
bloom_type **create_bloom_table(int, int);
void reset_bloom_table_row(uint32_t row, uint32_t size);
//void printBits(size_t const size, void const * const ptr);
void moveBloomFilter(uint32_t from_level, uint32_t from_page_num,uint32_t to_level, uint32_t to_page_num);
bloom_type **g_bloom_table;

#endif /* BLOOM_H */