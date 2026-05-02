#include <stdio.h>

unsigned char decode_gray_buffer(unsigned char *src, unsigned char *dst, size_t n)
{
    if (!n)
        return 0;
    for (int i = 0; i < n; i++)
    {
        src[i] ^= src[i] >> 4;
        src[i] ^= src[i] >> 2;
        src[i] ^= src[i] >> 1;
        dst[i] = src[i];
    }
    return dst[n - 1];
}

int main()
{
    unsigned char src[] = {5, 4};
    unsigned char dst[2];
    decode_gray_buffer(src, dst, 2);
    printf("%u %u\n", dst[0], dst[1]);
}