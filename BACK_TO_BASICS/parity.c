#include <stdio.h>

int parity(unsigned char n) {
    int count = 0;
    for(int i = 0; i <= 7; i++){
        count += ((n >> i) & 1);
    }
    return count & 1;
}

// oppure SWAR
// int parity(unsigned char n) {
//     n ^= n >> 4;
//     n ^= n >> 2;
//     n ^= n >> 1;
//     return n & 1;
// }
// supponiamo che il numero sia 10110100,cioe 180; la proprieta' base e' che lo xor di un numero con se stesso e' 0
// xor del numero con se stesso shiftato di 4 a destra,ovvero 00001011 , risultato 10111111,quindi 
// abbiamo xorato i bit del nible basso con quelli del nibble alto,quindi il bit 0 del risultato contiene bit0 ^ bit4, il bit 1 bit1 ^ bit5 ecc.
// poi xoriamo il risultato con il risultato stesso shiftato di due,cioe 00101111, risultato 10010000,quindi il bit0 adesso contiene bit0 ^ bit ^2 ^bit4 ^ bit ^6
//con il n ^=  n >> 1 il bit 0 del risultato alla fine contiene bit0 ^ bit1 ^ bit2 ^ bit3 ^ bit4 ^ bit5 ^ bit6 ^ bit7,cioe lo xor di tutti i bit,
//che e' esattamente la parita'
int main() {
    printf("%d\n", parity(0b11111111));//0
    printf("%d\n", parity(0b11111110));//1
    printf("%d\n", parity(0b11111100));//0
}