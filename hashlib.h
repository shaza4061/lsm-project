#include <stdint.h>
#define HASH_SEED 1
uint32_t mod_hash(uint32_t a, uint32_t N);
uint32_t bin_hash(uint32_t a, uint32_t N);
uint32_t midsquare_hash(uint32_t a);
uint32_t multiplication_hash(uint32_t key, uint32_t N);