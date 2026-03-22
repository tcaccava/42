#include <unistd.h>
#include <stdio.h>

void print(int *curr, int len)
{
    for (int i = 0; i < len; i++)
    {
        char c = curr[i] + '0';
        write(1, &c, 1);
    }
    write(1, "\n", 1);
}

void subseq(int *a, int size, int index, int *curr, int len)
{
    if (index == size)
    {
        print(curr, len);
        return;
    }
    //la questione e' solo una: per ogni numero dell'array prenderlo o no nell'array temporaneo?
    // non prendo a[index]
    subseq(a, size, index + 1, curr, len);

    // prendo a[index]
    curr[len] = a[index];
    subseq(a, size, index + 1, curr, len + 1);
}

int main() {
    int array[3] = {1,2,3};
    int curr[3];
    subseq(array,3,0,curr,0);
}