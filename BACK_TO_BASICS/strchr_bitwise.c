#include <stdio.h>
#include <stdint.h>

char *bitwise_strchr(const char *s, int c)
{
    const unsigned char *p = (const unsigned char *)s;
    unsigned char uc = (unsigned char)c;

    // replica il byte target su tutta una word da 64 bit
    uint64_t pattern = uc * 0x0101010101010101ULL;
    // allineamento
    while ((uintptr_t)p & 7)
    {
        if (*p == uc)
            return (char *)p;
        if (*p == 0)
            return NULL;
        p++;
    }
    while (1)
    {
        // legge 8 byte consecutivi
        uint64_t block = *(uint64_t *)p;

        // XOR con pattern → zero dove c è presente
        uint64_t x = block ^ pattern;

        // trucco SWAR per trovare zero
        uint64_t mask = (x - 0x0101010101010101ULL) & ~x & 0x8080808080808080ULL;

        if (mask)
        {
            // almeno un byte uguale a c trovato
            // dobbiamo capire quale
            for (int i = 0; i < 8; i++)
            {
                if (p[i] == uc)
                    return (char *)(p + i);
            }
        }

        // controlla se c era '\0'
        // se il blocco contiene zero terminatore, stop
        uint64_t zero_mask = (block - 0x0101010101010101ULL) & ~block & 0x8080808080808080ULL;
        if (zero_mask)
        {
            // scan finale dei byte fino al '\0'
            for (int i = 0; i < 8; i++)
            {
                if (p[i] == uc || p[i] == 0)
                    return (p[i] == uc) ? (char *)(p + i) : NULL;
            }
        }

        // passa al prossimo blocco di 8 byte
        p += 8;
    }
}

int main()
{
    printf("%s\n", bitwise_strchr("ciaone", 97));
}