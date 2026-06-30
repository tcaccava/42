/*
 * =====================================================================================
 * ZIGZAG_DECODE - RIPRISTINO VALORE SIGNED
 * =====================================================================================
 *
 * BACKGROUND TEORICO:
 * La decodifica inverte la mappatura ZigZag. Poiché il segno è stato codificato 
 * nel bit meno significativo (LSB), l'operazione di decodifica deve identificare 
 * se il numero era dispari (negativo) o pari (positivo) e ripristinare il segno.
 *
 * FORMULA: (n >> 1) ^ -(n & 1)
 * - (n >> 1): Divisione intera per 2.
 * - -(n & 1): Estrae il bit di segno e lo espande a una maschera (0x00 o 0xFF).
 * - XOR: Applica la maschera per ripristinare il valore originale in complemento a due.
 * =====================================================================================
 */

#include <stdio.h>

signed char zigzag_decode(unsigned char n)
{
    return ((n ^ ((n & 1) * 0xff)) >> 1) | (n << 7);
}

// oppure piu elegantemente
//  signed char zigzag_decode(unsigned char n) {
//      return (n >> 1) ^ -(n & 1);
//  }

int main()
{
    printf("%d\n", zigzag_decode(7)); //-4
    printf("%d\n", zigzag_decode(3)); //-2
    printf("%d\n", zigzag_decode(5)); //-3
    printf("%d\n", zigzag_decode(4)); // 2
}