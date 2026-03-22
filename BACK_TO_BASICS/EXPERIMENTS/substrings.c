#include <unistd.h>
#include <stdio.h>
#include <string.h>

void substr(char *s, int index, char *tmp, int tindex)
{
    if (index == strlen(s))
    {
        printf("%s\n", tmp);
        return;
    }
    substr(s, index + 1, tmp, tindex);
    tmp[tindex] = s[index];
    tmp[tindex + 1] = 0;//null terminator per ogni stringa tmp
    substr(s, index + 1, tmp, tindex + 1);
}

int main() {
    char *s = "abc";
    char tmp[4];
    substr(s,0,tmp,0);
}