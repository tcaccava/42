#include <stdio.h>
#include <unistd.h>
//identico a array num permutations,ma con il controllo della lunghezza
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

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void permut_without_rep(int *a, int size, int index,int length)
{
    if (index == length)
    {
        print(a, length);
        return;
    }
    int i = index;
    while (i < size)
    {
        swap(&a[index], &a[i]);
        permut_without_rep(a, size, index + 1,length);
        swap(&a[index], &a[i]);
        i++;
    }
}

int main()
{
    int a[] = {1, 2, 3};
    int tmp[2];
    permut_without_rep(a, 3, 0,2);
}