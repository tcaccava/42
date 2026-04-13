#include <stdio.h>

int ft_isalpha(int c)
{
    return (c >= 65 && c <= 90) | (c >= 97 && c <= 122);
}

int main()
{
    printf("%d %d %d\n", ft_isalpha(65), ft_isalpha(97), ft_isalpha(123));
}