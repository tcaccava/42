#include <stdio.h>
// estrae i bit di n nelle posizioni dove mask ha 1,e li compatta verso destra eliminando buchi
unsigned char compress_bits(unsigned char n, unsigned char mask)
{
    unsigned char result = 0;
    int j = 0;
    for (int i = 0; i <= 7; i++)
    {
        if (mask >> i & 1)
            result |= (n >> i & 1) << j++;
    }
    return result;
}
// int main()
// {
//     printf("%d\n", compress_bits(0b10110100, 0b01010101)); // 00000110
//     printf("%d\n", compress_bits(0b11000101, 0b01100101)); // 00001011
// }