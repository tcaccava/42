#include <stdio.h>
#include <unistd.h>

void ft_putnbr(int num)
{
    if (num == -2147483648)
    {
        write(1, "-2147483648", 11);
        return;
    }
    if (num < 0)
    {
        write(1, "-", 1);
        num = -num;
    }
    if (num > 9)
    {
        ft_putnbr(num / 10);
    }
    char c = num + '0';
    write(1, &c, 1);
}

void print_array(int *a, int size)
{
    if (size == 1)
    {
        ft_putnbr(a[0]);
        return;
    }
    print_array(a, size - 1);//prima fai ricorsione su cifre precedenti
    ft_putnbr(a[size - 1]);//poi stampi l'ultimo numero
    //con queste due righe in ordine inverso sarebbe un reverse print account
}

int main()
{
    int arr[] = {1, 2, 3, 4};
    print_array(arr, 4);
}