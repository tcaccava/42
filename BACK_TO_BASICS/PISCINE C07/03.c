#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ft_strjoin(int size, char **strs, char *sep)
{
    if (size == 0)
    {
        char *empty = malloc(1);
        if (!empty)
            return NULL;
        empty[0] = '\0';
        return empty;
    }
    int strs_mem = 0;
    for (int i = 0; i < size; i++)
    {
        strs_mem += strlen(strs[i]);
    }
    int sep_mem = strlen(sep) * (size - 1);
    int total = (strs_mem + sep_mem + 1);
    char *res = malloc(total);
    char *p = res;
    for (int i = 0; i < size; i++)
    {
        char *s = strs[i];
        while (*s)
            *p++ = *s++;
        if (i < size - 1)
        {
            char *t = sep;
            while (*t)
                *p++ = *t++;
        }
    }
    *p = '\0';
    return res;
}

int main()
{
    char *s1 = "ciao";
    char *s2 = "come";
    char *s3 = "va";
    char *strs[] = {s1, s2, s3};
    char *sep = "XXX";
    printf("%s\n", ft_strjoin(3, strs, sep));
}