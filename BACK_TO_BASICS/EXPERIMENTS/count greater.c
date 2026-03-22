#include <stdio.h>

int count_greater(int *a, int size, int target)
{
    if (size == 1)
        return (a[0] > target);
    return ((a[size - 1] > target) ? 1 : 0) + count_greater(a, size - 1, target);
}

int main() {
    int a[] = {2,3,44,53,6,7,91,43,22};
    printf("%d\n",count_greater(a,9,43));
}
//la ricorsione e' sempre caso base,contributo locale e chiamata sul resto