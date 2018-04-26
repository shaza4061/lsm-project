#include "bloom.h"
#include "hashlib.h"
#include <stdio.h>
#include <stdlib.h>

void moveBloomFilter(uint32_t from_level, uint32_t from_page_num,uint32_t to_level, uint32_t to_page_num){
	g_bloom_table[to_level][to_page_num] = g_bloom_table[from_level][from_page_num];
}
void addToBloom(uint32_t level, uint32_t page_num, uint32_t key) {
	uint32_t pos,filter_size = sizeof(bloom_type)*8;	
	
	pos = mod_hash(key,filter_size) ;
	g_bloom_table[level][page_num] |= (1<<pos);

//printBits(sizeof(g_bloom_table[level][page_num]), &g_bloom_table[level][page_num]);
	pos = bin_hash(key,filter_size) ;
	g_bloom_table[level][page_num] |= (1<<pos);
	
	pos = midsquare_hash(key);

	g_bloom_table[level][page_num]  |= (1<<(pos%filter_size));

}

int contains(uint32_t level, uint32_t page_num,uint32_t key) {
	int pos1,pos2,pos3,filter_size = sizeof(bloom_type)*8;
	
	pos1 = mod_hash(key,filter_size);
	pos2 = bin_hash(key,filter_size);
	pos3 = midsquare_hash(key);
	
	if (
		((g_bloom_table[level][page_num]) & (1<<pos1)) &&
		((g_bloom_table[level][page_num]) & (1<<pos2))&&
		((g_bloom_table[level][page_num]) & (1<<(pos3%filter_size)) ) 
			)
				return 1;
	else
		return 0;
	
}

bloom_type **create_bloom_table(int row, int col) /* Allocate the array for bloom filter*/
{
    /* Check if allocation succeeded. (check for NULL pointer) */
    bloom_type **table;
    table = malloc(row*sizeof(bloom_type *));
    for(int i = 0 ; i < row ; i++)
        table[i] = malloc( col*sizeof(bloom_type) );
    return table;
}

void reset_bloom_table_row(uint32_t row, uint32_t size){
	for (uint32_t i =0; i< size ; i++){
		g_bloom_table[row][i] = 0x0;
	}
	
}

//void printBits(size_t const size, void const * const ptr)
//{
//    unsigned char *b = (unsigned char*) ptr;
//    unsigned char byte;
//    int i, j;
//
//    for (i=size-1;i>=0;i--)
//    {
//        for (j=7;j>=0;j--)
//        {
//            byte = (b[i] >> j) & 1;
//            printf("%u", byte);
//        }
//    }
//    puts("");
//}