#include <stdio.h>
#include "./reverse_bits.c"
//inverte l'ordine dei nibble ma anche i bit dentro ogni nibble
unsigned char reverse_nibbles(unsigned char n){
    return reverse_bits(n);//e' esattamente uguale a reverse bits
}

int main() {
    printf("%d\n", reverse_nibbles(0b10110100));//00101101,cioe' 45
}