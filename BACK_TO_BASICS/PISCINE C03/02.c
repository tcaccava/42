#include <stdio.h>
#include <string.h>

char *ft_strcat(char *dest, char *src)
{
    char *start = dest;
    while (*dest)
        dest++;
    while (*src)
        *dest++ = *src++;
    *dest = 0;
    return start;
}

int main()
{
    char dest[9] = "ciao";
    char *add = "come";
    char *add2 = "va";
    printf("%s\n", ft_strcat(dest, add));
    // printf("%s\n", ft_strcat(dest,add2)); buffer overflow
}