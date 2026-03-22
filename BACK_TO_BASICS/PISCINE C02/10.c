#include <stdio.h>
#include <string.h>

// differenza con strncpy: copia al massimo size - 1, termina sempre con \0,niente padding,ritorna strlen(src)
// e' un alternativa moderna e sicura,no rischio buffer overflow
unsigned int ft_strlcpy(char *dest, const char *src, unsigned int size)
{
    if (size == 0)
        return strlen(src);
    int i = 0;
    while (src[i] && i < size - 1)
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = 0;
    return strlen(src);
}

int main()
{
    char buf[10];
    ft_strlcpy(buf, "ciaonestocazzo", sizeof(buf));
    printf("%s\n", buf); // ciaonesto
    strncpy(buf, "ciaonestocazzo", sizeof(buf));
    printf("%s\n", buf); // ciaonestoc
}