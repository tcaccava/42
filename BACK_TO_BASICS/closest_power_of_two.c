#include <stdio.h>
#include "./keep_highest_bit.c"
unsigned char closest_power_of_two(unsigned char n){
    if(n && !(n & (n - 1))) return n;//controlla se e' una potenza di 2 ed evita l'overflow con 0
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    return keep_highest_bit(n) << 1;
}

int main() {
    printf("%d\n", closest_power_of_two(0b01111111));
}