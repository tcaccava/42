#include <stdio.h>

char *ft_strncat(char *dest, char *src, unsigned int nb)
{
    int i = 0;
    while (dest[i])
        i++;
    int j = 0;
    while (src[j] && j < nb)
    {
        dest[i + j] = src[j];
        j++;
    }
    dest[i + j] = 0;
    return dest;
}

int main()
{
    char dest[12] = "ciao";
    char *add = "stupidocazzone";
    printf("%s\n", ft_strncat(dest, add, 7));
}