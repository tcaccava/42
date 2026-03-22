#include <unistd.h>
#include <stdlib.h>

int ft_atoi(char *s)
{
    int result = 0;
    int sign = 1;
    while (*s == '+' || *s == '-')
    {
        if (*s == '-')
            sign = -sign;
        s++;
    }
    while (*s >= '0' && *s <= '9')
    {
        result = result * 10 + (*s - '0');
        s++;
    }
    return result * sign;
}

int ft_strcmp(char *s, char *op)
{
    while (*s && *op && *s == *op)
    {
        s++;
        op++;
    }
    return (unsigned char)*s - (unsigned char)*op;
}

void ft_putnbr(int n)
{
    char c;
    if (n < 0)
    {
        write(1, "-", 1);
        if (n == -2147483648)
        {
            write(1, "2147483648", 10);
            return;
        }
        n = -n;
    }
    if (n >= 10)
        ft_putnbr(n / 10);
    c = n % 10 + '0';
    write(1, &c, 1);
}

int sum(int a, int b) { return a + b; }
int diff(int a, int b) { return a - b; }
int molt(int a, int b) { return a * b; }
int divi(int a, int b) { return a / b; }
int mod(int a, int b) { return a % b; }

void do_op(int a, int b, char *op)
{
    char *op_str[] = {"+", "-", "*", "/", "%"};
    int (*arr[5])(int, int) = {sum, diff, molt, divi, mod};
    int i = 0;
    while (i < 5)
    {
        if (!ft_strcmp(op_str[i], op))
            ft_putnbr(arr[i](a, b));
        i++;
    }
}

int main(int c, char **v)
{
    if (c != 4)
        return 1;

    int a = ft_atoi(v[1]);
    int b = ft_atoi(v[3]);
    if (ft_strcmp(v[2], "+") && ft_strcmp(v[2], "-") && ft_strcmp(v[2], "*") && ft_strcmp(v[2], "/") && ft_strcmp(v[2], "%"))
    {
        write(1, "0\n", 2);
        return 1;
    }
    if (!ft_strcmp(v[2], "/") && b == 0)
    {
        write(2, "Stop : division by zero\n", 24);
        return 1;
    }
    if (!ft_strcmp(v[2], "%") && b == 0)
    {
        write(2, "Stop : modulo by zero\n", 23);
        return 1;
    }
    do_op(a, b, v[2]);
}
