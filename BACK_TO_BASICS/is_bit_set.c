#include <stdio.h>

int is_bit_set(unsigned char n, int i){
    return (n >> i) & 1;
}

int main(){
    printf("%d\n", is_bit_set(3, 1));
    printf("%d\n", is_bit_set(3, 0));
    printf("%d\n", is_bit_set(3, 2));
}