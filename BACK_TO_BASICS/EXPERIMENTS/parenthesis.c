#include <unistd.h>
#include <stdlib.h>

void print(char *s, int index)
{
    int i = 0;
    while (i < index)
    {
        write(1, &s[i], 1);
        i++;
    }
    write(1, "\n", 1);
}

void parenthesis(int n, int index, char *tmp,int sx_count, int dx_count)
{
    char sx = '(';
    char dx = ')';
  
    if (sx_count == n && dx_count == n)
    {
        print(tmp, index);
        return;
    }

    if (sx_count < n)
    {
        tmp[index] = sx;
        parenthesis(n, index + 1, tmp,sx_count + 1,dx_count);
    }
    if (dx_count < sx_count)
    {
        tmp[index] = dx;
        parenthesis(n, index + 1, tmp,sx_count,dx_count + 1);
    }
}

int main()
{
    int n = 3;
    char *tmp = malloc(n * 2);
    parenthesis(n, 0, tmp,0,0);
}