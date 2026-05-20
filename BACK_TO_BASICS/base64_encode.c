#include <stdio.h>
#include "./printbits.c"
void module_0(unsigned char **src, size_t src_len, unsigned char **dst)
{
    while (src_len >= 3)
    {
        **dst = (**src & 252) >> 2;
        *dst += 1;
        **dst = (**src & 3) << 4 | ((*(*src + 1) & 0xF0) >> 4);
        *dst += 1;
        **dst = ((*(*src + 1) & 0x0F) << 2) | ((*(*src + 2) & 192) >> 6);
        *dst += 1;
        **dst = *(*src + 2) & 63;
        *dst += 1;
        if (src_len == 3)
            break;
        *src += 3;
        src_len -= 3;
    }
}

void module_1(unsigned char *src, size_t src_len, unsigned char **dst)
{
    **dst = (*src & 252) >> 2;
    *dst += 1;
    **dst = (*src & 3) << 4;
    *dst += 1;
    **dst = 61;
    *dst += 1;
    **dst = 61;
}

void module_2(unsigned char *src, size_t src_len, unsigned char **dst)
{
    **dst = (*src & 252) >> 2;
    *dst += 1;
    **dst = ((*src & 3) << 4) | ((*(src + 1) & 0xF0) >> 4);
    *dst += 1;
    **dst = (*(src + 1) & 0x0F) << 4;
    *dst += 1;
    **dst = 61;
}
void base64_encode(unsigned char *src, size_t src_len, unsigned char *dst)
{
    static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    if (!(src_len % 3))
        module_0(&src, src_len, &dst);
    else if (src_len % 3 == 1)
    {
        module_0(&src, src_len, &dst);
        module_1(src, src_len, &dst);
    }
    else if (src_len % 3 == 2)
    {
        module_0(&src, src_len, &dst);
        module_2(src, src_len, &dst);
    }
}

int main()
{
    unsigned char dst[4];
    base64_encode("Man", 3, dst);
    for (int i = 0; i < 4; i++)
        printbits(dst[i]);
    base64_encode("M", 1, dst);
    for (int i = 0; i < 4; i++)
        printbits(dst[i]);
    base64_encode("Ma", 2, dst);
    for (int i = 0; i < 4; i++)
        printbits(dst[i]);
}