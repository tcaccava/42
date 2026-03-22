#include <string.h>
#include <unistd.h>

// Controlla se la base è valida:
// - almeno 2 caratteri
// - nessun duplicato
// - nessun '+' o '-'

int is_valid_arg(char *base)
{
    int len = strlen(base);
    if (len < 2)
        return 0;
    for (int i = 0; i < len; i++)
    {
        if (base[i] == '-' || base[i] == '+')
            return 0;
        for (int j = i + 1; j < len; j++)
        {
            if (base[i] == base[j])
                return 0;
        }
    }
    return 1;
}

void ft_putnbr_base(int nbr, char *base)
{
    unsigned int n = 0;
    int base_len = strlen(base);
    if (!is_valid_arg(base))
        return;
    if (nbr < 0)
    {
        write(1, "-", 1);
        n = -nbr; // usando un unsigned int posso evitare di trattare separatamente
    } // il caso di INT MIN
    else
        n = nbr;
    if (n >= base_len)
        ft_putnbr_base(n / base_len, base);
    char c = base[n % base_len];
    write(1, &c, 1);
}

int main()
{
    
    ft_putnbr_base(255, "00123456789ABCDEF");  // wrong
    ft_putnbr_base(255, "0123456789ABCDEF"); // FF
    write(1, "\n", 1);
    ft_putnbr_base(-42, "01"); // -101010
    write(1, "\n", 1);
    ft_putnbr_base(125, "poneyvif"); // vif
    write(1, "\n", 1);
    ft_putnbr_base(123, "0123"); // 1023
    write(1, "\n", 1);
    return 0;
}
