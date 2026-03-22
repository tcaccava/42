#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void print(int *tmp, int tmp_index)
{
    int i = 0;
    while (i < tmp_index)
    {
        char c = tmp[i] + '0';
        write(1, &c, 1);
        i++;
    }
    write(1, "\n", 1);
}

int one_occ(int *a, int len)
{
    int i = 0;
    int occ = 0;
    while (i < len)
    {
        if (a[i] == 1)
            occ++;
        i++;
    }
    return occ;
}

void binary_seq(int a, int b, int *tmp, int k, int len, int tindex)
{
    if (tindex == len)
    {
        if (k == 0)
            print(tmp, tindex);
        return;
    }
    if (k > 0)
    {
        tmp[tindex] = b;
        binary_seq(a, b, tmp, k - 1, len, tindex + 1);
    }
    tmp[tindex] = a;
    binary_seq(a, b, tmp, k, len, tindex + 1);
}

int main()
{
    int tmp[4];
    binary_seq(0, 1, tmp, 2, 4, 0);
}