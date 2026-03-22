#include <string.h>
#include <stdio.h>
//copia tutta src,compreso \0,nullatermina dest,non controlla se il buffer e' sufficiente,rischio buffer
//overflow
char *ft_strcpy(char *dest, const char *src)
{
    int i = 0;
    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = 0;
    return dest;
}

int main()
{
    char buf[5];
    char *str = "ciao";
    ft_strcpy(buf, str);
    printf("%s\n", buf);
    strcpy(buf, str);
    printf("%s\n", buf);
}