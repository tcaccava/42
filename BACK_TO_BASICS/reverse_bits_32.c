#include <stdio.h>

unsigned int ft_reverse_bits32(unsigned int n)
{
    n = ((n & 0xAAAAAAAA) >> 1) | ((n & 0x55555555) << 1);   // scambia bit adiacenti
    n = ((n & 0xCCCCCCCC) >> 2) | ((n & 0x33333333) << 2);   // scambia coppie di bit
    n = ((n & 0xF0F0F0F0) >> 4) | ((n & 0x0F0F0F0F) << 4);   // scambia nibbles
    n = ((n & 0xFF00FF00) >> 8) | ((n & 0x00FF00FF) << 8);   // scambia bytes adiacenti
    n = ((n & 0xFFFF0000) >> 16) | ((n & 0x0000FFFF) << 16); // scambia halfword
    return n;
}

int main()
{
    printf("%d \n", ft_reverse_bits32(0X0F000000)); // 240
    printf("%d \n", ft_reverse_bits32(0XF0000000)); // 15
}