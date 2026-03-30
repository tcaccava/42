#include <stdio.h>
// accende tutti i bit dal piu' basso acceso fino al bit 0 e spegne tutti gli altri,usato in networking per calcolare subnet mask
//  e in compilatori per allineare strutture dati
unsigned char fill_from_lowest(unsigned char n)
{
    n |= n << 1;
    n |= n << 2;
    n |= n << 4;
    return ~(n << 1);
}

int main()
{
    printf("%d\n", fill_from_lowest(0b10110100)); // 0b00000111
    printf("%d\n", fill_from_lowest(0b00000001)); // 0b00000001
    printf("%d\n", fill_from_lowest(0b10000000)); // 0b11111111
    printf("%d\n", fill_from_lowest(0b00001000)); // 0b00001111
}