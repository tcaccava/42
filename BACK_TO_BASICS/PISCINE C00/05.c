#include <unistd.h>
void ft_print_comb()
{
    char a = 48, b, c;
    while (a <= 55)
    {
        b = a + 1;
        while (b <= 56)
        {
            c = b + 1;
            while (c <= 57)
            {
                write(1, &a, 1);
                write(1, &b, 1);
                write(1, &c, 1);
                if (!(a == '7' && b == '8' && c == '9'))
                    write(1, ", ", 2);
                c++;
            }
            b++;
        }
        a++;
    }
}
int main()
{
    ft_print_comb();
}