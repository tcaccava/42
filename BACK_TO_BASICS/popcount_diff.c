#include <stdio.h>
#include "./popcount_swar32.c"
// Ritorna la differenza tra il popcount di due interi,positiva se a ha piu' bit accesi,negativa
//  in caso contrario

int popcount_diff(unsigned int a, unsigned int b)
{
    return popcount_swar32(a) - popcount_swar32(b);
}

int main()
{
    printf("%d %d\n", popcount_diff(240, 15), popcount_diff(128, 127)); // 0 -6
}