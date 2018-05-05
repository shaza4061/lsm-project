#include "bloom.h"
#include "hashlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// static bloom_filter** g_bloom_table;
void initBloomFilter(int level)
{
    uint32_t max_page_size = g_bloom_filter_ptr[level].max_page_size;
    g_bloom_filter_ptr[level].page = malloc(max_page_size * (sizeof(bloom_filter_bitmap) + 2));
    for(int j = 0; j < (int)max_page_size; j++)
	g_bloom_filter_ptr[level].page[j].bitmap =
	    calloc(g_bloom_filter_ptr[level].filter_size / 8, sizeof(bloom_type));
}
void moveBloomFilter(uint32_t from_level, uint32_t from_page_num, uint32_t to_level, uint32_t to_page_num)
{

    if(g_bloom_filter_ptr[to_level].page == NULL) {
	initBloomFilter(to_level);
    }
    for(int bitmap_index = 0; bitmap_index < g_bloom_filter_ptr[from_level].filter_size / 8; bitmap_index++) {
	g_bloom_filter_ptr[to_level].page[to_page_num].bitmap[bitmap_index] =
	    g_bloom_filter_ptr[from_level].page[from_page_num].bitmap[bitmap_index];
	g_bloom_filter_ptr[from_level].page[from_page_num].bitmap[bitmap_index] = 0;
    }
}
void addToBloom(uint32_t level, uint32_t page_num, uint32_t key)
{
    uint32_t pos, filter_size, bitmap_index, bit_index;
    filter_size = g_bloom_filter_ptr[level].filter_size;
    if(g_bloom_filter_ptr[level].page == NULL) {
	initBloomFilter(level);
    }

    pos = mod_hash(key, filter_size);
    bitmap_index = pos / (sizeof(bloom_type) * 8);
    bit_index = pos % (sizeof(bloom_type) * 8);
    g_bloom_filter_ptr[level].page[page_num].bitmap[bitmap_index] |= (1 << bit_index);

    pos = bin_hash(key, filter_size);
    bitmap_index = pos / (sizeof(bloom_type) * 8);
    bit_index = pos % (sizeof(bloom_type) * 8);
    g_bloom_filter_ptr[level].page[page_num].bitmap[bitmap_index] |= (1 << bit_index);

    pos = multiplication_hash(key, filter_size);
    bitmap_index = pos / (sizeof(bloom_type) * 8);
    bit_index = pos % (sizeof(bloom_type) * 8);
    g_bloom_filter_ptr[level].page[page_num].bitmap[bitmap_index] |= (1 << bit_index);
}

int contains(uint32_t level, uint32_t page_num, uint32_t key)
{
    uint32_t pos1, pos2, pos3, filter_size;
    uint32_t bitmap_index1, bitmap_index2, bitmap_index3;
    uint32_t bit_index1, bit_index2, bit_index3;
    int is_bit1_set, is_bit2_set, is_bit3_set;
    filter_size = g_bloom_filter_ptr[level].filter_size;

    pos1 = mod_hash(key, filter_size);
    bitmap_index1 = pos1 / (sizeof(bloom_type) * 8);
    bit_index1 = pos1 % (sizeof(bloom_type) * 8);
    is_bit1_set = g_bloom_filter_ptr[level].page[page_num].bitmap[bitmap_index1] & (1 << bit_index1);

    pos2 = bin_hash(key, filter_size);
    bitmap_index2 = pos2 / (sizeof(bloom_type) * 8);
    bit_index2 = pos2 % (sizeof(bloom_type) * 8);
    is_bit2_set = g_bloom_filter_ptr[level].page[page_num].bitmap[bitmap_index2] & (1 << bit_index2);

    pos3 = multiplication_hash(key, filter_size);
    bitmap_index3 = pos3 / (sizeof(bloom_type) * 8);
    bit_index3 = pos3 % (sizeof(bloom_type) * 8);
    is_bit3_set = g_bloom_filter_ptr[level].page[page_num].bitmap[bitmap_index3] & (1 << bit_index3);

    if(is_bit1_set && is_bit2_set && is_bit3_set)
	return 1;
    else
	return 0;
}
