#include <stdio.h>

int is_uppercase(char c)
{
    if (c >= 65 && c <= 90)
        return 1;
    return 0;
}

char *ft_strupcase(char *str)
{
    char *start = str;
    while (*str)
    {
        if (!is_uppercase(*str))
            *str = *str - 32;
        str++;
    }

    return start;
}

int main()
{
    char uppercase[] = "CIAO";
    char lowercase[] = "ciao";
    char mix[] = "CIao";
    printf("%s\n", ft_strupcase(uppercase));
    printf("%s\n", ft_strupcase(lowercase));
    printf("%s\n", ft_strupcase(mix));
}