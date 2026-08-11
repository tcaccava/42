#include <stdio.h>

// Prende un array di n valori ciascuno tra 0 e 15 e li impacchetta in un uint32_t: 
// arr[0] nei bit 0-3, arr[1] nei bit 4-7, e così via fino a 8 elementi. Esempio: arr = {0xA, 0x5, 0x3, 0x7}, n = 4 → 0x00007 35A.
unsigned int pack_nibbles(unsigned char *arr, int n) {
    __uint32_t res = 0;
    unsigned int mask = 0xf;
    unsigned int shift = 0; 
    for(int i = 0; i < n; i++){
        res |= (arr[i] << shift)

    }