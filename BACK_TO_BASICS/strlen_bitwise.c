#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

size_t strlen_hex(const char *s)
{
    const char *p = s;

    // allineamento fino a 8 byte
    while (((uintptr_t)p) & 7)
    {
        if (*p == 0)
            return p - s;
        p++;
    }

    while (1)
    {
        uint64_t v = *(uint64_t *)p;

        // formula bitwise 64-bit con esadecimali
        if ((v - 0x0101010101010101ULL) & ~v & 0x8080808080808080ULL)
        {
            while (*p++);
            return --p - s;
        }
        p += 8;
    }
}
// int main()
// {
//     printf("%zu\n", strlen_hex("ciaoarpgnrpgnrpougnqpoorgnnqowruugqpogqerpogeoguqroguunqerpoug"));
// }