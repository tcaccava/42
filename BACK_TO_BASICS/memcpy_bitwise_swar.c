#include <stdio.h>
#include <stdint.h>
#include "./printbits.c"
//memcpy non gestisce l'overlap,deve essere il chiamante a garantire che dst e src non siano sovrapposti
//in caso contrario potrebbe leggere bytes che ha gia' sovrascritto == undefined behavior
unsigned char *ft_memcpy(unsigned char *dst, const unsigned char *src, size_t n)
{
    unsigned char *d = dst;

    // fase 1: byte per byte fino all'allineamento di dst a 8
    while (n > 0 && ((uintptr_t)d & 7))
    {
        *d++ = *src++;
        n--;
    }

    // fase 2: SWAR — copia 8 byte alla volta
    while (n >= 8)
    {
        *(uint64_t *)d = *(uint64_t *)src;
        d += 8;
        src += 8;
        n -= 8;
    }

    // fase 3: byte per byte per i rimanenti
    while (n > 0)
    {
        *d++ = *src++;
        n--;
    }

    return dst;
}

// int main()
// {
//     unsigned char src[] = {0xa5, 0xdd, 0xff};
//     unsigned char dest[3];
//     ft_memcpy(dest, src, 3);
//     printbits(dest[0]);
//     printbits(dest[1]);
//     printbits(dest[2]);
// }