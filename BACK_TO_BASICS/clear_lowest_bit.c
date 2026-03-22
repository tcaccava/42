#include <stdio.h>

unsigned char clear_lowest_bit(unsigned char n) {
    return n & (n - 1);
}

int main() {
    printf("%d\n", clear_lowest_bit(0b01010100)); //01010000,cioe' 80
    printf("%d\n", clear_lowest_bit(0b00011000)); //00010000, cioe 16; 
}