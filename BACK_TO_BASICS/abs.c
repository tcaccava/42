#include <stdio.h>

int ft_abs(int n) {
    return n ^ (n ^ (~n + 1)) & -(n <= 0);
}
//oppure sfrutto il fatto che il MSB di un intero a 32 bit e' per il segno
//int ft_abs(int n) {
//    int mask = n >> 31;
//    return (n ^ mask) - mask;
//}
//prima spalmo il bit del segno sull'intero numero per ottenere una maschera
//se il numero e' positivo mask e' 0,l'OR di n con zero da n,meno zero ancora n
//se e' negativo invece la mask e' 0xFFFFFFFF,che e' -1,l'OR con una maschera simile
//da -n + 1,e quindi -n - 1 -(-1),cioe' n
int main() {
    printf("%d %d %d %d %d\n",ft_abs(-100), ft_abs(2), ft_abs(-32), ft_abs(0), ft_abs(-1));
}