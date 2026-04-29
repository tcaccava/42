#include <stdio.h>
#include <stdint.h>
// Nei video ogni frame è simile al precedente — invece di salvare ogni frame intero, salvi solo le differenze.
// Delta encoding fa esattamente questo sui byte. Usato dai codec di compressione in audio/video.

void delta_decode(unsigned char *src, unsigned char *dst, size_t n)
{
    if (n <= 1)
    {
        if (n == 1)
            *dst = *src;
        return;
    }
    *dst++ = *src++;
    n--;
    while (n--)
    {
        *dst = *src++ + *(dst - 1);
        dst++;
    }
}