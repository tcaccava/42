#include <stdio.h>
//prende i 4 bit bassi di n e li sparge nei bits pari del risultato ,mettendo 0 nei dispari
//usato in hash table e algoritmi di sorting a basso livello
unsigned char spread_bits(unsigned char n) {
   unsigned char result = 0;
   for(int i = 0; i < 4; i++){
    result |= ((n >> i) & 1) << (i * 2);
   }
   return result;
}
//oppure piu' elegantemente
// unsigned char interleave_zero(unsigned char n) {
//     return ((((n & 1) | ((n & 2) << 1)) | ((n & 4) << 2)) | ((n & 8) << 3));
// }
//oppure la incomprensibile versione SWAR
// unsigned char spread_bits(unsigned char n) {
//     n &= 0x0F;
//     n = ((n & 0x0C) << 2) | (n & 0x03);
//     n = ((n & 0x22) << 1) | (n & 0x11);
//     return n;
// }
int main() {
    printf("%d\n", spread_bits(0b00001011));//01000101, cioe' 69

}