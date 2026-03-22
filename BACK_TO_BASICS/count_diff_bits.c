#include <stdio.h>

int count_diff_bits(unsigned char a, unsigned char b)
{
    unsigned char xored = a ^ b;
    int count = 0;
    for (int i = 0; i <= 7; i++)
    {
        count += (xored >> i) & 1;
    }
    return count;//la conta dei bit differenti e' chiamata distanza di Hamming
}

int main()
{
    printf("%d\n", count_diff_bits(0b00001111, 0b00000000));
    printf("%d\n", count_diff_bits(0b10101010, 0b01010101));
    printf("%d\n", count_diff_bits(0b11111111, 0b11111111));
}