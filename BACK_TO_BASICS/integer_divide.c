#include <stdio.h>
#include "./popcount_swar32.c"
// Calcola a / b usando solo shift, confronti e sottrazioni, senza / né %.
// Ritorna 0 se b = 0. Esempio: (17, 3) → 5, (16, 4) → 4.

static unsigned int msb(unsigned int n)
{
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n - (n >> 1);
}

unsigned int integer_divide(unsigned int a, unsigned int b)
{
    if (!b)
        return -1;
    if (!a || a < b)
        return 0;

    if (a == b || (a < (b << 1)))
        return 1;

    unsigned int shift = 0;
    unsigned int a_or = a;
    unsigned int a_msb = msb(a);
    unsigned int a_msb_or = a_msb;
    unsigned int b_msb = msb(b);
    unsigned int a_rest = a - a_msb;
    unsigned int b_rest = b - b_msb;
    unsigned int b_msb_pos = popcount_swar32(b_msb - 1);
    while (a > b)
    {
        a >>= 1;
        shift++;
    }
    if (a == b && (a_or == (a << shift)))
        return 1U << shift;
    unsigned int max = (1U << shift);
    a_rest = (a_rest == 0) ? (a_or >> 1) : a_rest;
    return max - (integer_divide(a_rest, b));
}

int main()
{
    printf("%u %u %u %u\n", integer_divide(64, 7), integer_divide(51, 3), integer_divide(59, 3), integer_divide(64, 8));
}

/*

msb = 2^n
d = b - msb
b = msb + d
a/b = a/(msb + d)
msb = b - d
d = b - msb
a/b = a/(b - d + b -msb)
a/b = a/(2b -d -msb)
1/b = 1/(2b - d - msb)


64 8 max 8 sov = 0
64 7 max 16 perche 16 x 4 = 64 sov = 7 32 7 max 8 sov = 4  16 7 max 4 sov = 2 8 7 = 1
64 15 max 8 perche 8 * 8 = 64 sov = 4
64 31 max 4 perche 4 * 16 = 4 sov = 2
63 15 max 4 sov = 2
78 15 max 8
79 15 max 8
80 15 max 8
120 15 max 8 perche' 8 x 15 = 120
123 15 max 8
128 15 max 16 perche 16 x 8 = 128


ma
127 15 max 16 perche 16 * 8 = 128
127 8  max 16 perche 16 * 8 = 128
*/