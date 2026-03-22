#include <stdio.h>

int is_lower(char c)
{
    if (c >= 97 && c <= 122)
        return 1;
    return 0;
}

int ft_str_is_lowercase(char *str)
{
    if (!str)
        return 1;
    while (*str)
    {
        if (!(is_lower(*str)))
            return 0;
        str++;
    }
    return 1;
}

int main()
{
    char *right = "abcdefghilmnopqrstuvz";
    char *wrong = "Abcdefghilmnopqrtuvz";
    printf("%d\n", ft_str_is_lowercase(right));
    printf("%d\n", ft_str_is_lowercase(wrong));
}