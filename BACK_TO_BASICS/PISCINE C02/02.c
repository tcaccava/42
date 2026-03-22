#include <stdio.h>

int is_alpha(char c)
{
    return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122));
}

int ft_str_is_alpha(char *str)
{
    if (!str)
        return 1;
    while (*str)
    {
        if (!is_alpha(*str))
            return 0;
        str++;
    }
    return 1;
}

int main()
{
    char *right = "abcdefghilmnopqrstuvwxyz";
    char *wrong = "abcdefghilmnopqrstuvwxy2";
    printf("%d\n", ft_str_is_alpha(right));
    printf("%d\n", ft_str_is_alpha(wrong));
}