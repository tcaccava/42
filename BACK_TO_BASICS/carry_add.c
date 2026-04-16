#include <stdio.h>
//algoritmo basato sul lavoro di claude Shannon e george boole,simulazione software del
//circuito fsico chiamat Ripple Carry Adder(Sommatore a propagazione di riporto) che fa le addizioni 
// nelle ALU,unita' aritmetico logiche della cpu
unsigned char ft_carry_add(unsigned char a, unsigned char b, unsigned char *carry)
{
    unsigned char and = 0;
    unsigned char prev = b;
    while (a)
    {
        and = a & b;//l'and seleziona dove i due addendi hanno entrambi bytes accesi,che quindi generano un riporto
        b = a ^ b;//lo xor segnala dove i due addendi differiscono,quindi genera una somma senza riporto,cioe calcola la somma di ogni colonna come se i riporti non esistessero
        a = and << 1;//il riporto generato in una colonna si somma a quella successiva,da qui lo shift a sinistra di 1
    }
    *carry = b < prev;
    return b;
}

    //versione meno efficiente con ricorsione
    // unsigned char ft_carry_add(unsigned char a, unsigned char b, unsigned char *carry)
    // {
    //     unsigned char and = a & b;
    //     unsigned char or = a | b;
    //     if (!and)
    //     {
    //         return or + *carry;
    //     }
    //     int i = 0;
    //     for (; i <= 7; i++)
    //     {
    //         if (and >> i & 1)
    //         {
    //             and ^= 1 << i;
    //             break;
    //         }
    //     }
    //     or ^= 1 << i++;
    //     for (; (or >> i & 1); i++)
    //         or ^= 1 << i;
    //     *carry = i == 8;
    //     or ^= 1 << i;
    //     return ft_carry_add(and, or, carry);
    // }

    int main()
{
    unsigned char carry = 0;
    printf("%d\n", ft_carry_add(131, 127, &carry));
    printf("%d\n", carry);
}