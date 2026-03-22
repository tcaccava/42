#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int is_sep(char c, char *charset)
{
    while (*charset)
        if (*charset++ == c)
            return 1;
    return 0;
}

int count_words(char *s, char *charset)
{
    int count = 0;
    while (*s)
    {
        while (*s && is_sep(*s, charset))
            s++;
        if (!*s)
            break;
        if (!is_sep(*s, charset))
            count++;
        while (*s && !is_sep(*s, charset))
            s++;
    }
    return count;
}

char *substr(char *s, int begin, int end)
{
    char *res = malloc(end - begin + 1);
    if (!res)
        return NULL;
    int i = 0;
    while (begin < end)
        res[i++] = s[begin++];
    res[i] = '\0';
    return res;
}

char **ft_split(char *str, char *charset)
{
    char **matrix = malloc(sizeof(char *) * (count_words(str, charset) + 1));
    if (!matrix)
        return NULL;
    int m = 0;
    int i = 0;
    while (str[i])
    {
        while (str[i] && is_sep(str[i], charset))
            i++;
        if (!str[i])
            break;
        int begin = i;
        while (str[i] && !is_sep(str[i], charset))
            i++;
        matrix[m++] = substr(str, begin, i);
    }
    matrix[m] = NULL;
    return matrix;
}

int main()
{
    char *str = ",:!ciao, come , va";
    char *charset = ",! :";
    char **matrix = ft_split(str, charset);
    int i = 0;
    while (matrix[i])
    {
        printf("%s\n", matrix[i]);
        free(matrix[i]);
        i++;
    }
    free(matrix);
    return 0;
}