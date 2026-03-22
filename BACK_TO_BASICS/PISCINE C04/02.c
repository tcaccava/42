#include <stdio.h>
#include <unistd.h>

// un int puo contenere valori da -2147483648(INT MIN) a 2147483647(INT MAX)
// se gli passi proprio l'int min,putnbr fa nb = -nb, cioe +2147483648->BUFFER OVERFLOW
// per questo si tratta separatamente il caso dell'int min
// l'alternativa sarebbe convertire nb ad unsigned int,che va da 0 al doppio di INT MAX,quindi
// puo contenere l'int max senza andare in buffer overflow
void ft_putnbr(int nb)
{
    if (nb == -2147483648)
    {
        write(1, "-2147483648", 11);
        return;
    }
    if (nb < 0)
    {
        write(1, "-", 1);
        nb = -nb;
    }

    if (nb >= 10)
        ft_putnbr(nb / 10);

    char c = (nb % 10) + '0';
    write(1, &c, 1);
}

int main()
{
    ft_putnbr(42);
    ft_putnbr(-42);
    ft_putnbr(-2147483648);
}