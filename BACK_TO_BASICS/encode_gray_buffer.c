#include <stdio.h>

unsigned char encode_gray_buffer(unsigned char *src, unsigned char *dst, size_t n)
{
    if (!n)
        return 0;
    for (int i = 0; i < n; i++)
        dst[i] = src[i] ^ (src[i] >> 1);
    return dst[n - 1];
}

int main()
{
    unsigned char src[] = {6, 7};
    unsigned char dst[2];
    encode_gray_buffer(src, dst, 2);
    printf("%u %u\n", dst[0], dst[1]);
}