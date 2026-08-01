#include <stdio.h>
// Calcola a / b usando solo shift, confronti e sottrazioni, senza / né %.
// Ritorna 0 se b = 0. Esempio: (17, 3) → 5, (16, 4) → 4.
unsigned int integer_divide(unsigned int a, unsigned int b)
{
    if (b > a || !b)
        return 0;
    unsigned int res = 0;
    unsigned int or_a = a;
    while(a > b){
        res ++;
    }
    

    
    
 
}

int main()
{
    printf("%d %d\n", integer_divide(17, 3), integer_divide(16, 4));
}

110010
000111

110100
000111

101010
000101

000101
000101