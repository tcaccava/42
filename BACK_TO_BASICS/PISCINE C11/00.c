#include <unistd.h>

void ft_foreach(int *tab, int length, void (*f)(int))
{
    int i = 0;
    while (i < length)
        f(tab[i++]);
}
// un puntatore a funzione si dichiara indicando prima il ritorno della funzione puntata,poi il nome del puntatore,quindi il tipo
// di argomento
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
    int result = 0;
    if (num >= 10)
    {
        ft_putnbr(num / 10);
    }
    char digit = num % 10 + '0';
    write(1, &digit, 1);
}

int main()
{
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    void (*f)(int) = ft_putnbr;
    ft_foreach(arr, 8, f);
}