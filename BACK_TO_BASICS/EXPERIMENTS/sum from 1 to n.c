#include <stdio.h>

int from_one_to_n(int n)
{
    if (n == 1)
        return 1;
    return n + from_one_to_n(n - 1);
}

int main()
{
    printf("%d\n", from_one_to_n(5));
}