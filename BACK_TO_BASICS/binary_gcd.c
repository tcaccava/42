#include <stdio.h>
// Massimo comune divisore usando solo shift e sottrazioni, senza mai usare % o /.
unsigned int binary_gcd(unsigned int a, unsigned int b)
{
    unsigned int min = b ^ ((a ^ b) & -(a < b));
    unsigned int or_min = min;
    unsigned int max = b ^ ((a ^ b) & -(b == min));
    unsigned int or_diff = max - min;
    

}       

int main()
{
    printf("%u\n", binary_gcd(77, 11));
    printf("%u\n", binary_gcd(77, 14));
    printf("%u\n", binary_gcd(35, 15));
}

0101
1111

