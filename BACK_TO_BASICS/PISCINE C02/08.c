#include <stdio.h>

int is_lowercase(char c)
{
    return (c >= 97 && c <= 122);
}

char *ft_strlowcase(char *str)
{
    char *start = str;
    while (*str)
    {
        if (!is_lowercase(*str))
            *str = *str + 32;
        str++;
    }
    return start;
}

int main()
{
    char uppercase[] = "ciao";
    char lowercase[] = "CIao";
    char mix[] = "CIAO";
    printf("%s\n", ft_strlowcase(uppercase));
    printf("%s\n", ft_strlowcase(lowercase));
    printf("%s\n", ft_strlowcase(mix));
}