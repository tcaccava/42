#include <stdio.h>
#include "./abs_diff.c"
// calcola la media di due numeri senza overflow
unsigned char average(unsigned char a, unsigned char b)
{
    return (a / 2 + b / 2) ^ ((((a + b) / 2) ^ (a / 2 + b / 2)) & -(a + b > a));
}
// oppure piu' elegantemente 
//  unsigned char average(unsigned char a, unsigned char b)
// {
//     return (a & b) + ((a ^ b) >> 1);
// }
// l'and tra a e b calcola la parte comune dei due numeri,cioe' i bit che sono 1 in entrambi
// lo xor shiftato a destra di 1 prende i bit diversi e li dimezza. Sommati danno la media esatta
// senza mai andare in overflow perche' non sommo mai a e b direttamente
int main()
{
    printf("%d\n", average(127, 127));
    printf("%d\n", average(127, 3));
    printf("%d\n", average(255, 255));
    printf("%d\n", average(200, 201));
    printf("%d\n", average(1, 2));
}