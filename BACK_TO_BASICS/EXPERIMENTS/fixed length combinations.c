#include <unistd.h>

void print(int *a, int len)
{
    int i = 0;
    while (i < len)
    {
        char c = a[i] + '0';
        write(1, &c, 1);
        i++;
    }
    write(1, "\n", 1);
}

void subseq(int *a, int size, int index, int *tmp, int tmpindex, int seqlength)
{
    if (tmpindex == seqlength)//pronta
    {
        print(tmp, tmpindex);
        return;
    }

    if (index == size) // niente più elementi → stop
        return;

    // NON prendere a[index]
    subseq(a, size, index + 1, tmp, tmpindex, seqlength);

    // prendere a[index]
    tmp[tmpindex] = a[index];
    subseq(a, size, index + 1, tmp, tmpindex + 1, seqlength);
}


int main()
{
    int a[] = {1, 2, 3, 4};
    int tmp[4];
    subseq(a, 4, 0, tmp, 0, 2);
}