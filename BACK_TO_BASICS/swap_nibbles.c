#include <stdio.h>

unsigned char swap_nibbles(unsigned char n) {
    return n = (n << 4 | n >> 4);
}

int main() {
    unsigned char byte = 0b01101001; // 105
    printf("%d\n", swap_nibbles(byte)); // 10010110, cioe 150
    printf("%d\n", swap_nibbles(0b00111001)); //10010011,cioe' 147
}