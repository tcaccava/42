#include <stdio.h>

// Rotation right (ROR)
unsigned int ror32(unsigned int x, int n)
{
    unsigned int shift = (unsigned int)n & 31;
    return (x >> shift) | (x << ((32 - shift) & 31));
}

// int main()
// {
//     unsigned int val = 0x00FF00FF;

//     printf("Originale: %#x\n", val);
//     printf("ROL 8:     %#x\n", rol32(val, 8));  // 0xff00ff00
//     printf("ROL 32:    %#x\n", rol32(val, 32)); // 0xff00ff (identico)
//     printf("ROL 64:    %#x\n", rol32(val, 64)); // 0xff00ff (identico)
//     printf("ROL -1:    %#x\n", rol32(val, -1)); // Ruota a sinistra di 31 (equivalente a ROR 1)

//     return 0;
// }