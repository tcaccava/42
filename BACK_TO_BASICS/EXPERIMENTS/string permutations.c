#include <unistd.h>
#include <stdio.h>
#include <string.h>

void swap(char *a, char *b)
{
    char tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void str_permutations(char *s, int index, int len)
{
    if (index == len)
    {
        printf("%s\n", s);
        return;
    }

    for (int i = index; i < len; i++)
    {
        swap(&s[index], &s[i]);
        str_permutations(s, index + 1, len);
        swap(&s[index], &s[i]);
    }
}

int main()
{
    char s[] = "abc";
    str_permutations(s, 0, strlen(s));
}