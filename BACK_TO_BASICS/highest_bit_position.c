#include <stdio.h>

int highest_bit_position(unsigned char n) {
    if(!n) return -1;
    int i = 7;
    for(; !(n & (1 << i)); i--);
    return i;
}

int main() {
    printf("%d\n", highest_bit_position(0b01000000));
    printf("%d\n", highest_bit_position(0b11001100));
}