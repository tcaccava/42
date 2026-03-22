#include <stdio.h>

int are_digits_increasing(int num)
{
    if (num < 10)
        return 1;
    int last_digit = num % 10;
    int previous_digit = (num / 10) % 10;
    if (last_digit < previous_digit)
        return 0;
    return are_digits_increasing(num / 10);
}

int main()
{
    printf("%d\n", are_digits_increasing(1234));
    printf("%d\n", are_digits_increasing(1243));
}