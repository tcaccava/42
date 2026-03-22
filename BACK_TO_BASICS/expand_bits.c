#include <stdio.h>
// opposto di bit gather, espande  nelle posizioni dove mask ha 1
unsigned char expand_bits(unsigned char n, unsigned char mask)
{
    unsigned char compressed = 0;
    unsigned char result = 0;
    int j = 0;
    for (int i = 0; i <= 7; i++)
    {
        if (mask >> i & 1)
            result |= (n >> j++ & 1) << i;
    }
    return result;
}

int main() {
    printf("%d\n", expand_bits(0b00000110, 0b01010101));//00010100
}