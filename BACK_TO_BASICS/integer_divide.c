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
    shift = 0;
    while (a_msb > b_msb)
    {
        a_msb >>= 1;
        shift++;
    }
    unsigned int res = 1U << shift;
    return res - (integer_divide(a_rest, b));
}

int main()
{
    printf("%u %u\n", integer_divide(49, 3), integer_divide(16, 4));
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

/*
111111
100000
*/