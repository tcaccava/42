#include <unistd.h>

void ft_putnbr(int nb){
    char c;

    if (nb == -2147483648)
    {
        write(1, "-2147483648", 11);
        return;
    }

    if (nb < 0)  // se il numero è negativo
    {
        write(1, "-", 1);
        nb = -nb;
    }
    if(nb >= 10)
        ft_putnbr(nb / 10);
    c = (nb % 10) + '0';
    write(1, &c, 1);
}
int main() {
    ft_putnbr(987);
}

//prima chiamata, 987 > 10,quindi seconda chiamata su 98,maggiore di 10,quindi
//terza chiamata su 9,che e' minore di 10,quindi viene stampato 9; la call stack quindi 
//ritorna alla seconda chiamata su 98,quindi (98 % 10) + '0',quindi viene stampato 8,quindi
//la call stack risale alla prima chiamata,cioe 987, (987 % 10) + '0' ,stampa 7.