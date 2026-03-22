#include <stdio.h>

unsigned char fill_from_highest(unsigned char n) {
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    return n;
}

int main() {
    printf("%d\n", fill_from_highest(0b00101000));// 001111111,cioe' 63;
}