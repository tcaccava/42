#include <stdio.h>

unsigned char keep_highest_bit(unsigned char n) {
    int i = 0;
    while(!((n << i++) & 128));
    return 1 << 8 - i;
}

// keep_highest_bit: azzera tutti i bit tranne il piu' alto acceso
//
// esempio: 00101000 → 00100000
//
// FASE 1: propaga i bit verso destra fino a riempire tutti i bit
//         sotto il bit piu' alto. ogni passo raddoppia la propagazione.
//         1+2+4 = 7, che e' la distanza massima dal bit piu' alto al bit 0.
//
// 00101000  (n originale)
// ----------------------------------------------------------------
// n |= n >> 1   → propaga di 1
// 00101000
// 00010100  (n >> 1)
// --------
// 00111100
// ----------------------------------------------------------------
// n |= n >> 2   → propaga di 2 (ora copre 3 posizioni: 1+2)
// 00111100
// 00001111  (n >> 2)
// --------
// 00111111
// ----------------------------------------------------------------
// n |= n >> 4   → propaga di 4 (ora copre 7 posizioni: 1+2+4)
// 00111111
// 00000011  (n >> 4)
// --------
// 00111111  (gia' completo in questo caso, ma serve per bit alti)
// ----------------------------------------------------------------
//
// FASE 2: ora n e' una maschera di 1 continua dal bit piu' alto fino al bit 0.
//         sottrai n >> 1 — cioe' la stessa maschera senza il bit piu' alto.
//         rimane solo il bit piu' alto.
//
// 00111111  (n)
// 00011111  (n >> 1)
// --------
// 00100000  ← solo il bit piu' alto ✓
 
// unsigned char keep_highest_bit(unsigned char n) {
//     n |= n >> 1;  // propaga di 1
//     n |= n >> 2;  // propaga di 2
//     n |= n >> 4;  // propaga di 4 → tutti i bit sotto il piu' alto sono accesi
//     return n - (n >> 1);  // rimuovi tutto tranne il bit piu' alto
// }
 

// int main() {
//     printf("%d\n", keep_highest_bit(0b00101001));// 00100000
//     printf("%d\n", keep_highest_bit(0b00010000));// 00010000quindi
// }


