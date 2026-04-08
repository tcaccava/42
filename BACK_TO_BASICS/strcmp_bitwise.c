#include <stdio.h>
#include <stdint.h>
int ft_strcmp(const char *s1, const char *s2)
{
    // allineamento
    while ((uintptr_t)s1 & 7 && *s1 && *s2)
    {
        if (*s1 ^ *s2)
            return (unsigned char)*s1 - (unsigned char)*s2;
        s1++;
        s2++;
    }
    // SWAR
    while (1)
    {
        uint64_t a = *(uint64_t *)s1;
        uint64_t b = *(uint64_t *)s2;
        uint64_t zero = (a - 0x0101010101010101ULL) & ~a & 0x8080808080808080ULL;
        if (zero || a ^ b) // se s1 e' null terminator o diverso da s2 blocca il ciclo while
            break;
        s1 += 8;
        s2 += 8;
    }
    // byte per byte per trovare la posizione esatta
    while (*s1 && !(*s1 ^ *s2))
    {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

int main()
{
    printf("%d\n", ft_strcmp("lollazzo", "lollazzo"));
    printf("%d\n", ft_strcmp("sollazzo", "lollazzo"));
}