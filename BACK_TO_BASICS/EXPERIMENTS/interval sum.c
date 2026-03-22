#include <stdio.h>

int interval_sum(int m, int n)
{
    if (m > n)
        return 0;
    if(m == n)
        return m;
    int sum = m + n;
    return sum + (interval_sum(m + 1, n - 1));
}

int main()
{
    printf("%d\n", interval_sum(2, 8));
}