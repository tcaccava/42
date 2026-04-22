#include <stdio.h>
#include <stdint.h>
#include <string.h>
// Run-Length Encoding ,comprime una sequenza di byte sostituendo sequenze ripetute con count e valore: es :AAABBC -> 3A2B1C
// E' la base di compressori come PackBits usato in TIFF e PDF

void search_bits(unsigned char **src, unsigned char **dst, size_t n, unsigned char *count, unsigned char *bytes_written, unsigned char *previous)
{
    while (n--)
    {
        if (**src == *previous)
            *count += 1;
        else
        {
            **dst = *count;
            (*dst)++;
            **dst = *previous;
            (*dst)++;
            *previous = **src;
            *bytes_written += 2;
            *count = 1;
        }
        (*src)++;
    }
}

unsigned char ft_encode_rle(unsigned char *src, unsigned char *dst, size_t n)
{
    unsigned char count = 1;
    unsigned char bytes_written = 0;
    unsigned char previous = *src;
    src++;
    n--;
    while (n && (uintptr_t)src & 7)
    {
        if (*src == previous)
            count++;
        else
        {
            *dst++ = (unsigned char)count;
            *dst++ = previous;
            previous = *src;
            bytes_written += 2;
            count = 1;
        }
        n--;
        src++;
    }
    while (n >= 8)
    {
        uint64_t block = *(uint64_t *)src;
        unsigned char c = previous;
        uint64_t mask = c * 0x0101010101010101;
        uint64_t diff = block ^ mask;
        if ((diff - 0x0101010101010101) & ~diff & 0x8080808080808080)
        {
            search_bits(&src, &dst, 8, &count, &bytes_written, &previous);
            n -= 8;
        }
        else
        {
            count += 8;
            src += 8;
            n -= 8;
        }
    }
    if (n)
        search_bits(&src, &dst, n, &count, &bytes_written, &previous);
    if (count)
    {
        *dst++ = count;
        *dst++ = previous;
        bytes_written += 2;
    }
    return bytes_written;
}
int main()
{
    unsigned char src[] = "ccccccccccccccccciiiiiiiiiaaaaaaallllllllllllllllllllllzxyxxoop";
    unsigned char dst[30];
    printf("%d\n", ft_encode_rle(src, dst, strlen(src)));
    for(int i = 0; i < 20; i++){
        printf("%u ", dst[i]);
    }
}