#include <stdio.h>

unsigned int rol32(unsigned int n, int k)
{
    unsigned int shift = (unsigned int)k & 31; // usiamo il resto di 32 come protezione nel caso n >= 32 o < 0
    // perche' non semplicemente 32 - shift? perche' ,se k fosse 0,32 meno shift sarebbe 32
    // e lo shift di un intero di 32 bit di una quantita >= alla sua dimensione e' in C undefined behavior
    // aggiungendo & 31 torna ad essere 0
    // dato un numero x e un numero n,l'operazione x & (n - 1) equivale a x % n solo se n e' una potenza di 2
    return (n << shift) | (n >> ((32 - shift) & 31));
}

int main()
{
    printf("%u\n", rol32(0b00000000111111110000000011111111, 8));
    printf("%#x\n", rol32(0x00FF00FF, 8));
    printf("%#x\n", rol32(0x00ff00ff, 64));
}
