#include <stdio.h>

unsigned char reverse_bits(unsigned char n)
{
    unsigned char rev = 0;
    for (int i = 0; i <= 7; i++)
    {
        unsigned char bit = (n >> i) & 1;
        unsigned char maschera = bit << 7 - i;
        rev = rev | maschera;
    }
    return rev;
}
// versione SWAR 
// unsigned char reverse_bits(unsigned char n)
// {
//     n = ((n & 0x55) << 1) | ((n & 0xAA) >> 1);
//     n = ((n & 0x33) << 2) | ((n & 0xCC) >> 2);
//     n = ((n & 0x0F) << 4) | ((n & 0xF0) >> 4);
//     return n;
// }

// int main()
// {
//     printf("%d\n", reverse_bits(15)); // 00001111 diventa 11110000,cioe 240
//     printf("%d\n", reverse_bits(23));// 00010111 diventa 11101000,cioe 232
//     printf("%d\n", reverse_bits(193));// 11000001 diventa 10000011,cioe 131
// }