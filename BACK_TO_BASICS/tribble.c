#include <stdio.h>
//prende i due bit bassi di n e li espande in 6 bit,replicando ogni bit 3 volte
unsigned char tribble(unsigned char n) {
    return (n & 192) | (n & 1) * 0b00000111 | (((n >> 1) & 1) * 0b00111000);
}

int main() {
    printf("%d\n", tribble(0b00000010));//00111000,cioe' 
}