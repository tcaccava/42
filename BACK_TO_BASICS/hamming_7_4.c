#include <stdio.h>

int parity_check(unsigned char a, unsigned char b,unsigned char c){
    return ()
}

void hamming_encode(unsigned char data, unsigned char *out){
    data &= 0x0F;
    *out = 0;
    *out = ((data & 1) << 2) | ((data & 2) << 4) | ((data & 4) << 5) | ((data & 8) << 6);
    (*out )
}