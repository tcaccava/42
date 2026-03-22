#include <stdio.h>

int is_upper(char c)
{
    if (c >= 65 && c <= 90)
        return 1;
    return 0;
}

int ft_str_is_lowercase(char *str)
{
    if (!str)
        return 1;
    while (*str)
    {
        if (!(is_upper(*str)))
            return 0;
        str++;
    }
    return 1;
}

int main()
{
    char *right = "ABCDEFGHILMNOPQRSTUVZ";
    char *wrong = "ABCDEFGHILMNOPQRSTUVz";
    printf("%d\n", ft_str_is_lowercase(right));
    printf("%d\n", ft_str_is_lowercase(wrong));
}