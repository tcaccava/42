#include <stdio.h>
#include <stdint.h>
char *ft_strncat(char *dst, const char *src, size_t n)
{
    char *d = dst;
    //allineamento ad 8
    while ((uintptr_t)dst & 7)
    {
        if (!(*dst))//esci se trovi il null terminator
            break;
        dst++;
    }
    //SWAR
    while (42)
    {
        uint64_t block = *(uint64_t *)dst;
        if ((block - 0x0101010101010101ULL) & ~block & 0x8080808080808080ULL)
        {
            for (int i = 0; i < 8; i++)
            {
                if (!(*(dst + i)))
                {
                    dst += i;
                    break;
                }
            }
            break;
        }
        dst += 8;
    }
    //copia
    while (n && *src)
    {
        *dst++ = *src++;
        n--;
    }
    *dst = '\0';
    return d;
}

int main()
{
    char dst[30] = "balulz";
    printf("%s\n", ft_strncat(dst, "precipitevolissimevolmente", 8));
}