#include <stdio.h>
#include <stdint.h>

int ft_memcmp(const unsigned char *s1, const unsigned char *s2, size_t n)
{
    // FASE 1: byte per byte fino all'allineamento a 8
    // ((uintptr_t)s & 7) e' il numero di byte mancanti all'allineamento
    while (n > 0 && ((uintptr_t)s1 & 7 || (uintptr_t)s2 & 7))
    {
        if (*s1 != *s2)
            return *s1 - *s2;
        s1++;
        s2++;
        n--;
    }

    // FASE 2: SWAR — 8 byte alla volta
    while (n >= 8)
    {
        __uint64_t block1 = *(__uint64_t *)s1;
        __uint64_t block2 = *(__uint64_t *)s2;
        __uint64_t diff = block1 ^ block2;

        if (diff)
        {
            for (int i = 0; i < 8; i++)
                if (s1[i] != s2[i])
                    return s1[i] - s2[i];
        }
        s1 += 8;
        s2 += 8;
        n -= 8;
    }

    // FASE 3: byte per byte per i byte rimanenti (al massimo 7)
    while (n > 0)
    {
        if (*s1 != *s2)
            return *s1 - *s2;
        s1++;
        s2++;
        n--;
    }

    return 0;
}

int main()
{
    unsigned char s1[] = {0x01, 0xff, 0x6a};
    unsigned char s2[] = {0x01, 0xff, 0x5b};
    printf("%d\n", ft_memcmp(s1, s2, 3));
}