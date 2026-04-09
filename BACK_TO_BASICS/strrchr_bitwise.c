#include <stdio.h>
#include <stdint.h>
char *ft_strrchr(const char *s, int c)
{
    char *begin = (char *)s;
    char *index = NULL;
    unsigned char uc = (unsigned char)c;
    uint64_t pattern = uc * 0x0101010101010101ULL;
    while ((uintptr_t)begin & 7)
    {
        if (*begin == uc)
            index = begin;
        if (!*begin)
            return index;
        begin++;
    }
    while (42)
    {
        uint64_t block = *(uint64_t *)begin;
        uint64_t x = block ^ pattern;
        uint64_t mask = (x - 0x0101010101010101ULL) & ~x & 0x8080808080808080ULL;
        if (mask)
        {
            for (int j = 0; j < 8; j++)
            {
                if (!*(begin + j)) // terminatore trovato prima di c
                    return index;
                if (*(begin + j) == uc)
                    index = begin + j;
            }
        }
        uint64_t zero_mask = (block - 0x0101010101010101ULL) & ~block & 0x8080808080808080ULL;
        if (zero_mask)
        {
            for (int j = 0; j < 8; j++)
            {
                if (!*(begin + j))
                    return index;
            }
        }
        begin += 8;
    }
}

int main()
{
    printf("%s\n", ft_strrchr("ifqriguuqigqgqiurugiqqiyryryrynxaaaaaajqquqiaa", 'i'));
}