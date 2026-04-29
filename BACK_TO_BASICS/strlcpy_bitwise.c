#include <stdio.h>
#include <stdint.h>
#include "./strlen_bitwise.c"
size_t ft_strlcpy(char *dst, const char *src, size_t size)
{
    size_t len = strlen_hex(src); // calcolo la lunghezza di src,servira' alla fine per capire se c'e' stato troncamento o meno
    char *s = (char *)src;
    // allineamento ad 8
    while ((uintptr_t)s & 7 && *s && size > 1)
    {
        *dst++ = *s++;
        size--;
    }
    // SWAR
    while (size >= 9) // strlcpy garantisce la copia di size - 1 bytes da src a dst
    {
        uint64_t src_block = *(uint64_t *)s;
        // se nel blocco c'e' il null terminator
        if ((src_block - 0x0101010101010101ULL) & ~src_block & 0x8080808080808080ULL)
        {
            while (*s && size > 1)
            {
                *dst++ = *s++;
                size--;
            }
            break;
        }
        *(uint64_t *)dst = src_block; // copio a blocchi di 8 bytes
        dst += 8;
        s += 8;
        size -= 8;
    }
    while (*s && size > 1) // ultimi al piu' 8 bytes
    {
        *dst++ = *s++;
        size--;
    }
    *dst = '\0';
    return len;
}

int main()
{
    char dst[10];
    printf("%s %zu\n", dst, ft_strlcpy(dst, "lollazzobalulzlellmao", 11));
}