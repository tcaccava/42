#include <stdio.h>
#include <stdint.h>

int ft_strncmp(const char *s1, const char *s2, size_t n) {
    if(!n)
        return 0;
    // allineamento
    while ((uintptr_t)s1 & 7 && *s1 && *s2 && n)
    {
        if (*s1 ^ *s2)
            return (unsigned char)*s1 - (unsigned char)*s2;
        s1++;
        s2++;
        n--;
    }
    // SWAR
    while (n >= 8)
    {
        uint64_t a = *(uint64_t *)s1;
        uint64_t b = *(uint64_t *)s2;
        uint64_t zero = (a - 0x0101010101010101ULL) & ~a & 0x8080808080808080ULL;
        if (zero || a ^ b) // se s1 e' null terminator o diverso da s2 blocca il ciclo while
            break;
        s1 += 8;
        s2 += 8;
        n -= 8;
    }
    // byte per byte per trovare la posizione esatta
    while (*s1 && !(*s1 ^ *s2) && n)
    {
        s1++;
        s2++;
        n--;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

int main()
{
    printf("%d\n", ft_strncmp("lollazzoaaaaaaaaaabbbbn", "lollazzoaaaaaaaaaabbbbc",23));
    printf("%d\n", ft_strncmp("sollazzo", "lollazzo", 4));
}
