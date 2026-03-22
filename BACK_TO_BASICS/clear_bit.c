#include <stdio.h>

unsigned char clear_bit(unsigned char n, int i) {
    return (n & ~(1 << i));
}

int main() {
    unsigned char n = 15; // 00001111
    printf("%d\n", clear_bit(n, 3));//7
}