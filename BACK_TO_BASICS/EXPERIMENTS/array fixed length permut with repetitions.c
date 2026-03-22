#include <stdio.h>
#include <unistd.h>

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

void fixed_len_permut(int *a, int size, int index, int *tmp, int tindex, int seqlength)
{
    if (tindex == seqlength)
    {
        print(tmp, tindex);
        return;
    }
    int i = 0;
    while (i < size)
    {
        tmp[tindex] = a[i];
        fixed_len_permut(a, size, index + 1, tmp, tindex + 1, seqlength);
        i++;
    }
}

int main()
{
    int a[] = {1, 2};
    int tmp[3];
    fixed_len_permut(a, 2, 0, tmp, 0, 3);
}