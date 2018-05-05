#include "hashlib.h"
#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

uint32_t mod_hash(uint32_t a, uint32_t N) {
	return abs(a)%N;
}

uint32_t bin_hash(uint32_t a, uint32_t N) {
	return  abs(a)/(INT32_MAX/N);
}

uint32_t midsquare_hash(uint32_t a) {

	uint32_t b =  pow((long)a,2);
	int result;
	// length of 2**32 - 1, +1 for nul.
	char buff[11]={'\0'},key[3]={'\0'};

	// copy to buffer
	sprintf(buff, "%" PRIu32, b);	
	int len = strlen(buff);
	if(len%2==0){
		key[0] = buff[len/2];
		key[1] = buff[(len/2)+1];
	}else {
		key[0] = buff[len/2];
	}

	sscanf(key, "%d", &result);
	return result;
}

// Based on http://lcm.csa.iisc.ernet.in/dsa/node44.html
uint32_t multiplication_hash(uint32_t key, uint32_t N) {
	const double A = 0.6180339887;// (Golden Ratio)
	return floor(fmod(((double)key*A),1.0)*N);
}

int roundUp(int numToRound, int multiple)
{
    if (multiple == 0)
        return numToRound;

    int remainder = numToRound % multiple;
    if (remainder == 0)
        return numToRound;

    return numToRound + multiple - remainder;
}