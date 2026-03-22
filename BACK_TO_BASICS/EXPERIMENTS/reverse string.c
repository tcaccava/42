#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// void rev_str(char *s, int left, int right)
// {
//     if (left >= right)
//     {
//         printf("%s\n", s);
//         return;
//     }
//     char tmp;
//     tmp = s[left];
//     s[left] = s[right];
//     s[right] = tmp;
//     rev_str(s, left + 1, right - 1);
// }

// int main()
// {
//     char s[] = "ciao";
//     rev_str(s, 0, strlen(s) - 1);
// }

void rev_str(char *s)
{
    if (!*s)
        return;
    rev_str(s + 1); // prima ricorsione
    write(1, s, 1); // poi scrivi
    // se fai il contrario la stampa normale,non al contrario
}

int main()
{
    char *s = "ciao";
    rev_str(s);
}