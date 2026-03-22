#include <stdio.h>

unsigned char rotate_left(unsigned char n, int k)
{
    k %= 8;
    return (n << k) | (n >> (8 - k));
    // n << k sposta i bit a sinistra di k posizioni,i bit che escono vengono persi e a destra entrano zeri
    // per es. per k = 2 e n = 177 cioe 10110001 , n << 2 = 11000100
    //n >> (8 - k) prende gli stessi bit che sarebbero usciti e li sposta all'estrema destra
    // per k == 2 , 8 - k = 6, quindi n >> 6 == 00000010
    // OR di 11000100 su 00000010 fa 11000110,che e' 198,rotazione completa di 6 volte a sinistra
    //di 10110001
}


// oppure
// unsigned char rotate_left(unsigned char n, int k)
// {
//     while (k > 0)
//     {
//         if ((((n >> 7) & 1) - 1) & ~((n >> 7) & 1))
//             n = n << 1;
//         else
//         {
//             n = n << 1;
//             n = n | 1;
//         }
//         k--;
//     }
//     return n;
// }

// oppure
// unsigned char rotate_left(unsigned char n, int k)
// {

//     while (k > 0)
//     {
//         unsigned char bit_seven = n >> 7 & 1;
//         n = (n << 1) | bit_seven;
//         k--;
//     }
// }


int main()
{
    printf("%d\n", rotate_left(2, 5));   // 00000010 diventa 01000000,cioe 64
    printf("%d\n", rotate_left(3, 4));   // 00000011 diventa 00110000,cioe 48
    printf("%d\n", rotate_left(129, 4)); // 10000001 diventa 00011000,cioe 24
}
