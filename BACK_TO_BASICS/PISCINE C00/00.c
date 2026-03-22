#include <unistd.h> // libreria per write

void ft_putchar(char c)
{
    write(1, &c, 1); // write e' una system call
    // le sys call sono funzioni dell'os che permettono ai programmi di interagire col kernel o le risorse hw
}

int main()
{
    ft_putchar('A');
}