#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int digits_sum(int num)
{
    int sum = 0;
    if (num < 10)
        return num;
    sum = num % 10 + digits_sum(num / 10);
    return sum;
}

int main()
{
    printf("%d\n", digits_sum(123));
}