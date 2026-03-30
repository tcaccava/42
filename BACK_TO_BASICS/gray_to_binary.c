#include <stdio.h>
//converte il numero da codice Gray a binario
//per es. 011 gray diventa il suo binario corrispondente,cioe' 2, 010 diventa 3 ,011 4 e cosi via
//l'incomprensibile versione SWAR
unsigned char gray_to_binary(unsigned char n) {
    n ^= n >> 4;
    n ^= n >> 2;
    n ^= n >> 1;
    return n;
}
//oppure di bruteforce con la ricorsione e binary che parte da 0
//in bitwise a == b se a ^ b non e' zero,cioe' se !(a ^ b)
// unsigned char gray_to_binary(unsigned char n,unsigned char binary){
//     return (!(n ^ (binary ^ (binary >> 1)))) ? binary : gray_to_binary(n, binary + 1);
// }

int main() {
    printf("%d\n", gray_to_binary(0b00000001));
    printf("%d\n", gray_to_binary(0b00000010));
    printf("%d\n", gray_to_binary(0b00000011));
    printf("%d\n", gray_to_binary(0b00000110));
}