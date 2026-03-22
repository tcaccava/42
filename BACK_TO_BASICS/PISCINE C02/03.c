#include <stdio.h>

int is_num(char c)
{
    return ((c >= 48 && c <= 57));
}
int ft_str_is_numeric(char *str)
{
    if (!str)
        return 1;
    while (*str)
    {
        if (!is_num(*str))
            return 0;
        str++;
    }
    return 1;
}

int main()
{
    char *right = "0123456789";
    char *wrong = "0123456789a";
    printf("%d\n", ft_str_is_numeric(right));
    printf("%d\n", ft_str_is_numeric(wrong));
}