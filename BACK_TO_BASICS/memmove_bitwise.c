#include <stdio.h>
#include <stdint.h>
#include "./memcpy_bitwise_swar.c"
// a differenza di memcpy, gestisce correttamente il caso in cui
// src e dst si sovrappongono — memcpy ha undefined behavior in quel caso.
// il chiamante deve garantire che src e dst puntino a regioni valide di n byte.
unsigned char *ft_memmove(unsigned char *dst, const unsigned char *src, size_t n)
{
    unsigned int gap = (uintptr_t)dst > (uintptr_t)src ? (uintptr_t)dst - (uintptr_t)src : (uintptr_t)src - (uintptr_t)dst;
    unsigned char *d = dst;
    // se dest sta prima di d, o n e' inferiore a gap,o entrambi,copiamo tradizionalmente da sx a dx con memcpy
    if ((uintptr_t)d < (uintptr_t)src || n < gap)
        return ft_memcpy(dst, src, n);
    //se invece dst e' dopo src E i bytes da copiare sono maggiori del gap,quindi c'e' overlap,allora dobbiamo copiare da dx a sx
    //spostiamo dst alla fine della sezione di n bytes da copiare,ovviamente il chiamante deve garantire che ci siano almeno n - 1 bytes a destra di dst
    d += n - 1;
    src = (unsigned char *)src + n - 1;//spostiamo conseguentemente anche src per mantenere l'allineamento
    while (n > 0 && ((uintptr_t)d & 7))//allineamo dest alla piu' vicina posizione multipla di 8 e copiamo a ritroso
    {
        *d-- = *src--;
        n--;
    }

    while (n >= 8)//SWAR, visto che il cast a uint64 ci "proietta" di 8 bytes a dx,ci spostiamo prima conseguentemnente a sx
    {
        d -= 8;
        src -= 8;
        *(uint64_t *)d = *(uint64_t *)src;
        n -= 8;
    }

    while (n > 0)//gli ultimi bytes
    {
        *d-- = *src--;
        n--;
    }
    return dst;
}

int main()
{
    unsigned char buff[6] = {'a', 'b', 'c', 'd', 'e', 'f'};
    unsigned char *d = ft_memmove(&buff[2], &buff[1], 4);
    printf("%s\n", d);
}