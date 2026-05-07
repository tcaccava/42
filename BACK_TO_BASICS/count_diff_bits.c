#include <stdio.h>

int count_diff_bits(unsigned char a, unsigned char b)
{
    unsigned char xored = a ^ b;
    int count = 0;
    for (int i = 0; i <= 7; i++)
        count += (xored >> i) & 1;
    return count; // la conta dei bit differenti e' chiamata distanza di Hamming
}
// oppure riuso l'algoritmo di Brian Kernighan, che ha in vantaggio di ciclare solo tante volte quanti sono i bit accesi in diff
//  unsigned int count_diff_bits(unsigned int n1, unsigned int n2)
//  {
//      unsigned int diff = n1 ^ n2;
//      unsigned int count = 0;
//      while (diff > 0)
//      {
//          diff &= (diff - 1); // "Spegne" il bit più a destra impostato a 1
//          count++;
//      }
//      return count;
//  }

// int main()
// {
//     printf("%d\n", count_diff_bits(0b00001111, 0b00000000));
//     printf("%d\n", count_diff_bits(0b10101010, 0b01010101));
//     printf("%d\n", count_diff_bits(0b11111111, 0b11111111));
// }