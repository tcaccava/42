#include <stdio.h>
// chiamato anche Gosper's Hack,dato un numero n ritorna il prossimo numero con lo stesso numero di bit accesi
// usato in algoritmi combinatori per iterare su tutti i sottoinsiemi di una certa dimensione
int bit_count(unsigned char n)
{
    int count = 0;
    while (n)
    {
        n &= n - 1;
        count++;
    }
    return count;
}
unsigned char next_permutation_bits(unsigned char n)
{
    int count = bit_count(n);
    while (bit_count(++n) != count)
        ;
    return n;
}
// oppure direttamente il gosper hack
//  unsigned char next_permutation_bits(unsigned char n) {
//      unsigned char lowest  = n & (-n);           // isola bit più basso
//      unsigned char ripple  = n + lowest;          // propaga il carry
//      unsigned char ones    = n ^ ripple;          // trova i bit cambiati
//      ones = (ones >> 2) / lowest;                 // risistemalì a destra
//      return ripple | ones;
//  }
int main()
{
    printf("%d\n", next_permutation_bits(0b00010011));
    printf("%d\n", next_permutation_bits(0b00010101));
    printf("%d\n", next_permutation_bits(0b00010110));
}