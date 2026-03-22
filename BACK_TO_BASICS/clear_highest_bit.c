#include <stdio.h>

unsigned char clear_highest_bit(unsigned char n)
{
    unsigned char h = n;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    return h & (n >> 1);
}

int main() {
    printf("%d\n", clear_highest_bit(0b00101001));// 00001001,cioe' 9
    printf("%d\n", clear_highest_bit(0b00010000));// 00000000
}