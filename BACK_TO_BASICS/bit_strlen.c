#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

size_t strlen_hex(const char *s)
{
    const char *p = s;

    // allineamento fino a 8 byte
    while (((uintptr_t)p) % 8 != 0)
    {
        if (*p == 0)
            return p - s;
        p++;
    }

    const uint64_t *w = (const uint64_t *)p;

    while (1)
    {
        uint64_t v = *w;

        // formula bitwise 64-bit con esadecimali
        if ((v - 0x0101010101010101ULL) & ~v & 0x8080808080808080ULL)
            break;

        w++;
    }

    p = (const char *)w;

    while (*p)
        p++;

    return p - s;
}

int main()
{
    printf("%zu\n", strlen_hex("ciao"));
}