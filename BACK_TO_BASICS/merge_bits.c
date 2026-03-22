#include <stdio.h>
// prende i bit di a dove mask e' 0 e quelli di b dove mask ha 1
unsigned char merge_bits(unsigned char a, unsigned char b, unsigned char mask)
{
    unsigned char result = 0;
    for (int i = 7; i >= 0; i--)
    {
        if (((mask >> i) - 1) & ~(mask >> i))
            result = result | (a & (1 << i));
        else
            result = result | (b & (1 << i));
    }
    return result;
}

// oppure versione bitwise pura senza loop,algoritmo bitwise blend
unsigned char merge_bits(unsigned char a, unsigned char b, unsigned char mask)
{
    return (a & ~mask) | (b & mask);
}

int main()
{
    printf("%d\n", merge_bits(0b11110000, 0b00001111, 0b00111100)); // 204
    printf("%d\n", merge_bits(0b10010000, 0b00001110, 0b10111100)); //
}