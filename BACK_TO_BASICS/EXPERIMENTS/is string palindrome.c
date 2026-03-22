#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int palindrome(char *s, int sx, int dx)
{
    if (sx >= dx)
        return 1;
    if (s[sx] != s[dx])
        return 0;
    return palindrome(s, sx + 1, dx - 1);
}

int main()
{
    char s[] = "akka";
    char s2[] = "ciao";
    printf("%d\n", palindrome(s, 0, strlen(s) - 1));
    printf("%d\n", palindrome(s2, 0, strlen(s) - 1));
}