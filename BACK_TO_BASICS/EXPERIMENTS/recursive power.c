#include <stdio.h>

int rec_pow(int num, int exp)
{
    if (exp == 0)
        return 1;
    return num * rec_pow(num, exp - 1);
}

int main()
{
    printf("%d\n", rec_pow(2, 6));
}