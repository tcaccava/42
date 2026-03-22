#include <stdio.h>

int ft_strlen(char *str)
{
    int len = 0;
    while (*str)
    {
        len++;
        str++;
    }
    return len;
}

int main()
{
    char *str = "stocazzo";
    printf("String length is : %d\n", ft_strlen(str));
}