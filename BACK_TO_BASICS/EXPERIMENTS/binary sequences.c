#include <unistd.h>
#include <stdio.h>
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

void binary_subseq(int a, int b, int *tmp, int index, int len)
{
    if (index == len)
    {
        print(tmp, index);
        return;
    }
    tmp[index] = a;
    binary_subseq(a, b, tmp, index + 1, len);
    tmp[index] = b;
    binary_subseq(a, b, tmp, index + 1, len);
}

int main()
{
    int tmp[3];
    binary_subseq(0, 1, tmp, 0, 3);
}
