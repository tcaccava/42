#include <stdio.h>

// Estrae i bit di n nelle posizioni pari se start=0, nelle posizioni dispari se start=1, e li compatta a destra. 
// Esempio: n=0b10110100, start=0 estrae bit 0,2,4,6 → 0,1,1,0 → risultato 0b0110. È deinterleave_a o deinterleave_b 
// estesa a 32 bit — scrivila in modo generico con start.
unsigned int extract_alternating(unsigned int n, int start)  {
    unsigned int mask = 0x01010101 << (start & 1);
    unsigned int mask_of_mask = ~((1U << start) - 1);
    mask &= mask_of_mask;
    n &= mask;
    
}

int main() {
    printf("%#X\n", extract_alternating(0xFFFFFFFF, 0));
}