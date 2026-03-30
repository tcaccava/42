#include <stdio.h>
// conta quanti bit accesi consecutivi ci sono partendo dal MSB,base di molti algoritmi di compressione e codifica huffmann
int leading_ones(unsigned char n)
{
    return (n & 128) ? 1 + leading_ones(n << 1) : 0;
}

int main()
{
    printf("%d\n", leading_ones(0b11110000));
    printf("%d\n", leading_ones(0b11111111));
    printf("%d\n", leading_ones(0b01110000));
    printf("%d\n", leading_ones(0b11001100));
}