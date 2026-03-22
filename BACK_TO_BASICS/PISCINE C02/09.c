#include <stdio.h>

int is_alpha_num(char c)
{
    return ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122));
}

char *ft_strcapitalize(char *str)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] >= 65 && str[i] <= 90)
            str[i] += 32;
        if ((i == 0 || !is_alpha_num(str[i - 1])) && (str[i] >= 97 && str[i] <= 122))
            str[i] -= 32;
        i++;
    }
    return str;
}

int main()
{
    char str[] = "hi, how are you? 42words forty-two; fifty+and+one";
    printf("%s\n", ft_strcapitalize(str));
}