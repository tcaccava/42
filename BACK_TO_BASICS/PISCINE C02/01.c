#include <stdio.h>
#include <string.h>

//differenza con strcpy: copia al massimo n byte,se src < n fa padding con \0,se src >= n NON aggiunge \0
//non garantisce di nullaterminare dest
char *ft_strncpy(char *dest, const char *src, unsigned int n)
{
    int i = 0;
    while (src[i] && i < n)
    {
        dest[i] = src[i];
        i++;
    }

    while (i < n)
    {
        dest[i] = 0;//padding
        i++;
    }

    return dest;
}

int main()
{
    char buf[15];
    char *str = "stocazzone";
    int n = 7;
    ft_strncpy(buf, str, 14);
    printf("%s\n", buf);
    strncpy(buf, str, 14);
    printf("%s\n", buf);
}