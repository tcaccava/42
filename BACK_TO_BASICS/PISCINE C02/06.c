#include <stdio.h>

int is_print(char c)
{
    if (c >= 32 && c <= 126)
        return 1;
    return 0;
}

int ft_str_is_printable(char *str)
{
    if (!str)
        return 1;
    while (*str)
    {
        if (!is_print(*str))
            return 0;
        str++;
    }
    return 1;
}
int main()
{
    char *right = "!@#$^&*()_";
    char *wrong = "\t\t\t\t"; // four /tab
    printf("%d\n", ft_str_is_printable(right));
    printf("%d\n", ft_str_is_printable(wrong));
}