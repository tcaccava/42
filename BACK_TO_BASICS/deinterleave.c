#include <stdio.h>
//estrae i bit in posizione pari da n e li compatta a destra
unsigned char deinterleave_a(unsigned char n) {
    unsigned char result = 0;
    for(int i = 0, j = 0; i <= 6; i = i + 2)
        result |= (n >> i & 1) << j++;
    return result;
}

int main() {
    printf("%d\n", deinterleave_a(0b11100100));// 00001010,cioe' 10
}