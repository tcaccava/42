#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *ft_strstr(char *str, char *to_find)
{
    if (!to_find)
        return str;
    while (*str)
    {
        char *s = str;
        char *f = to_find;
        while (*s && *f && *s == *f)
        {
            s++;
            f++;
        }
        if (*f == '\0')
            return str;
        str++;
    }
    return NULL;
}

int main()
{
    char *str = "stociaocazzo";
    char *search = "ciao";
    char *wrong = "peppapig";
    printf("%s\n", ft_strstr(str, search));
    if (!ft_strstr(str, wrong))
        write(1, "no occurrence", 13);
}