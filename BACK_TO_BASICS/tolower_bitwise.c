#include <stdio.h>
//branchless programming
// int tolower_bitwise(int c){
//     return c ^ (((c + 32) ^ c) & -(c >= 65 && c <= 90));
// }
// oppure sfruttiamo il fatto che la conversione maiusc/minusc agisce su un solo
// byte,il 5. Se il carattere e'maiuscolo fa l'OR con 1 per accendere il byte 5; se 
// non e' maiuscolo l'OR mantiene spento un byte già spento
int tolower_bitwise(int c){
    return c | ((c >= 65 && c <= 90) << 5);
}
int main() {
    printf("%c %c\n", tolower_bitwise(65), tolower_bitwise(64));
}