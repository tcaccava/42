#include <stdio.h>
#include <stdint.h>
#include "./delta_decode.c"
// Nei video ogni frame è simile al precedente — invece di salvare ogni frame intero, salvi solo le differenze.
// Delta encoding fa esattamente questo sui byte. Usato dai codec di compressione in audio/video.
void delta_encode(unsigned char *src, unsigned char *dst, size_t n)
{
    if (n <= 1)
    {
        if (n == 1)
            *dst = *src;
        return;
    }
    *dst++ = *src++; // il primo byte deve essere uguale,abbiamo bisogno di un punto fermo da cui partire nella fase di decoding
    n--;
    while (n--)
    {
        *dst++ = *src - *(src - 1); // ogni byte di dst e' uguale alla differenza tra il byte attuale di src e il precedente
        src++;// non inseriamo volontariamente una protezione dall'underflow nella sottrazione,tanto nella fase di decode gli eventuali underflow verranno compensati da overflow
    }
}
int main()
{
    unsigned char src[] = {97, 99, 101, 103, 105, 107, 111};
    unsigned char dst[7];
    delta_encode(src, dst, 7);
    for (int i = 0; i < 7; i++)
        printf("%u ", dst[i]);// 97 2 2 2 2 2 4
    printf("\n");
    unsigned char src2[7];
    delta_decode(dst, src2, 7);
    for (int i = 0; i < 7; i++)
        printf("%u ", src2[i]);// 97 99 101 103 105 107 111,quindi se diamo la dst originale in pasto a decode come src,decode costruirà la src originale
}
