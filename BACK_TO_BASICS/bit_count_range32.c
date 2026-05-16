#include <stdio.h>
#include "./popcount_swar32.c"
int count_bits_in_range32(unsigned int n, int from, int to) {
    unsigned int mask = ((1 << from) - (1 << to)) 
    return popcount_swar32(n & mask);
}

int main() {
    printf("%d %d", count_bits_in_range32(0xFF00FF00, 16,8),count_bits_in_range32(0xFF0FF00F, 23, 0));
}