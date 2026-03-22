#include <stdio.h>

int rev_strlen(char *s)
{
    if (!*s)
        return 0;
    return 1 + rev_strlen(s + 1);
}

int main()
{
    char *s = "LOLLAZZO";
    printf("%d\n", rev_strlen(s));
}