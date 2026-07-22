#include <stdio.h>
// Calcola la radice quadrata intera di n usando solo shift e confronti, senza sqrt(). 
// Esempio: n=17 → 4, n=16 → 4, n=15 → 3. L'algoritmo parte dal bit più alto possibile 
// e lo accende o spegne in base al confronto col quadrato corrente.

static unsigned int keep_highest_bit(unsigned int n){
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n & (n >> 1);
}

unsigned int integer_sqrt(unsigned int n) {
    unsigned int res = 1;
    n >>= 1;
    


}