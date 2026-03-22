#include <unistd.h>

void ft_print_comb2()
{
    int a = 0, b;
    char digits[5];
    while (a <= 98)
    {
        b = a + 1;
        while (b <= 99)
        {
            digits[0] = (a / 10) + '0';
            digits[1] = (a % 10) + '0';
            digits[2] = ' ';
            digits[3] = (b / 10) + '0';
            digits[4] = (b % 10) + '0';
            write(1, digits, 5);
            if (!(a == 98 && b == 99))
                write(1, ", ", 2);
            b++;
        }
        a++;
    }
}

int main()
{
    ft_print_comb2();
}