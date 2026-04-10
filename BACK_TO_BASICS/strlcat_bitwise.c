#include <stdio.h>
#include <stdint.h>
#include "./strlen_bitwise.c"
// concatena al piu size - 1 bytes di src alla fine di dst,limitando la scrittura a size - strlen(dst) - 1 caratteri
// restituisce la lunghezza totale della stringa che ha tentato di creare,ovvero strlen(dst iniziale) + strlen(src)
// se il valore di ritorno e' superiore a size,c'e' stato troncamento
size_t ft_strlcat(char *dst, const char *src, size_t size)
{
    size_t len = strlen_hex(dst) + strlen_hex(src);  // lunghezza da ritornare alla fine
    while ((uintptr_t)dst & 7 && *dst && size > 1) // allineamento ad 8
        size--;
    while (size >= 9) // SWAR
    {
        uint64_t block = *(uint64_t *)dst;
        if ((block - 0x0101010101010101ULL) & ~block & 0x8080808080808080ULL) // se nel blocco c'e' il terminatore
        {
            while (*dst)
            {
                dst++; // raggiungo la fine
                size--;
            }
            break;
        }
        dst += 8;
        size -= 8;
    }
    while (size > 1 && *src) // copio
    {
        *dst++ = *src++;
        size--;
    }
    *dst = 0;
    return len;
}

int main()
{
    char dst[50] = "lollazzo";
    printf("%s\n %zu\n", dst, ft_strlcat(dst, "fnqiwgqwiigbiwqigbqwiiggqwiigqwiigqwiiggqwiigqwigiqwigi", sizeof(dst)));
}
