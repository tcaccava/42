#include <stdio.h>
//prende il nibble basso di a e di b,e ne fa interleaving alternandoli con a in posizioni pari e b dispari
unsigned char interleave_bits(unsigned char a, unsigned char b){
    unsigned char result = 0;
    for(int i = 0, j = 0; i <= 3;i++){
        result |= (a >> i & 1) << j++;
        result |= (b >> i & 1)  << j++;
    }
    return result;
}

int main() {
    printf("%d\n", interleave_bits(0b00001010, 0b00001100));
}