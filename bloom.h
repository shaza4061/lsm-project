#ifndef BLOOM_H
#define BLOOM_H

#include <stdint.h>
#define M 64   // Number of bits in the Bloom filter
#define DEFAULT_FILTER_SIZE 64
//Bloom filter configuration
//n = 10
//p = 0.05 (1 in 20)
//m = 64 (8B)
//k = 3

//typedef unsigned char bitmap;
//typedef struct bloom_filter_t {
//	bitmap*  bit_array;
//	uint32_t filter_size;
//}bloom_filter;
typedef unsigned char bloom_type;

typedef struct bloom_filter_bitmap_t {
	bloom_type* bitmap;
}bloom_filter_bitmap;

typedef struct bloom_filter_t {
	bloom_filter_bitmap* page;
	int filter_size;
	uint32_t max_page_size;
} bloom_filter;

bloom_filter *g_bloom_filter_ptr;

void addToBloom(uint32_t level, uint32_t page_num, uint32_t key);
int contains(uint32_t level, uint32_t page_num,uint32_t key);
void init_bloom_table(int row, int col, int number_of_byte);
//void reset_bloom_table_row(uint32_t row, uint32_t size);
//void printBits(size_t const size, void const * const ptr);
void moveBloomFilter(uint32_t from_level, uint32_t from_page_num,uint32_t to_level, uint32_t to_page_num);


#endif /* BLOOM_H */