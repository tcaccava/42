#include <stdio.h>
//scambia ogni coppia di bit adiacenti
unsigned char byte_reverse_pairs(unsigned char n) {
    return (n >> 1 & 0b01010101) | ((n << 1) & 0b10101010) ;
}

int main() {
    printf("%d\n", byte_reverse_pairs(0b01100001));//10010010, cioe' 146 
}