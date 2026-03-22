#include <stdio.h>
#include <string.h>

int ft_strcmp(char *s1, char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2; // unsigned char, 8 byte,nessuno per il segno,valore da 0 a 255
}

int main()
{
    char *s1 = "ciao";
    char *s2 = "ciao";
    char *s3 = "fiao";
    char *s4 = "aiao";
    printf("%d\n", ft_strcmp(s1, s2));
    printf("%d\n", ft_strcmp(s3, s2));
    printf("%d\n", ft_strcmp(s4, s2));
}