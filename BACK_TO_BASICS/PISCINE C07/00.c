#include <stdlib.h>
#include <stdio.h>
#include <string.h>
char *ft_strdup(const char *src)
{
    char *dup = malloc(sizeof(char) * (strlen(src) + 1));
    if (!dup)
        return NULL;
    char *tmp = dup;
    while (*src)
        *dup++ = *src++;
    *dup = 0;
    return tmp;
}
int main()
{
    char *s = "ciao";
    printf("%s\n", ft_strdup(s));
}