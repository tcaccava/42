#include <stdio.h>
// Massimo comune divisore usando solo shift e sottrazioni, senza mai usare % o /.
unsigned int binary_gcd(unsigned int a, unsigned int b)
{
    unsigned int min = b ^ ((a ^ b) & -(a < b));
    unsigned int or_min = min;
    unsigned int max = b ^ ((a ^ b) & -(b == min));
    unsigned int diff = max - min;
    unsigned int or_diff = diff;
    if (min == 1)
        min = 2;
    while (diff > min)
    {
        min <<= 1;
        diff -= min;
    }
    if (!diff)
        return or_min;
    binary_gcd(max, or_min >> 1);
}

int main()
{
    printf("%u\n", binary_gcd(77, 11));
    printf("%u\n", binary_gcd(77, 14));
}

// // 111000    1001101   1001101   0010000
// // 001000    0001011   0001110   0000110

// // 110000    1000010   0111111   0001010
// // 010000    0010110   0011100   0001100

// // 100000    0101100   0100111
// // 100000    0101100   0111000

// //                     1001101   0010000
// //                     0000111   0000011

// //                     1000110   0001101
// //                     0001110   0000110

// //                     0111000   0000111
// //                     0011100   0001100

// //                     0011100   0010000
// //                     0111000   0000001

// //                               0001111
// //                               0000010

// //                               0001101
// //                               0000100

// //                               0001001
// //                               0001000

// //                               0000001
// //                               0010000

//                                  0010000
                                    0000010

                                    0001110
                                    0000100

                                    0001010
                                    0001010