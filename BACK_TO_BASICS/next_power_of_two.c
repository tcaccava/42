#include <stdio.h>
#include "./keep_highest_bit.c"
//ritorna la piu' piccola potenza di 2 strettamente maggiore di n
unsigned char next_power_of_two(unsigned char n) {
    return (!n) | (keep_highest_bit(n) << 1);//il !n serve semplicemente a gestire il caso in cui n sia 0,caso in cui deve ritornare 1,cioe' 2 ^
}

int main () {
    printf("%d\n", next_power_of_two(5));
    printf("%d\n", next_power_of_two(128));
    printf("%d\n", next_power_of_two(127));
    printf("%d\n", next_power_of_two(8));
    printf("%d\n", next_power_of_two(0));   
}