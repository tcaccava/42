#include <stdio.h>

int are_digits_alt(int num)
{
    if (num < 10)
        return 1;
    int last_digit = num % 10;
    int previos_digit = (num / 10) % 10;
    if ((last_digit % 2 == 0 && previos_digit % 2 == 0) || (last_digit % 2 != 0 && previos_digit % 2 != 0))
        return 0;
    return are_digits_alt(num / 10);
}

int main()
{
    printf("%d\n", are_digits_alt(1234));
    printf("%d\n", are_digits_alt(1134));
}