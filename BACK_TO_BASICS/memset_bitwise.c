#include <stdio.h>
#include <stdint.h>
// riempie i primi n bytes di s con il valore c,e ritorna s.
unsigned char *ft_memset(unsigned char *s, unsigned char c, size_t n)
{
    unsigned char *str = s;
    while (n > 0 && ((uintptr_t)str & 7))
    {
        n--;
        *str++ = c;
    }
    uint64_t pattern = (uint64_t)c * 0x0101010101010101ULL;//per efficienza calcolo il pattern fuori dal ciclo while
    while (n >= 8)
    {
        *(uint64_t *)str = pattern;
        n -= 8;
    }

    while (n > 0)
    {
        *str++ = c;
        n--;
    }
    return s;
}

// int main()
// {
//     unsigned char s[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
//     printf("%s\n", ft_memset(s, 90, 4));
// }