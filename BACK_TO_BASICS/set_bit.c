#include <stdio.h>

unsigned char set_bit(unsigned char n, int i){
    return (n | (1 << i));
}

int main() {
    int n = 8;
    printf("%d\n", set_bit(n, 4));//24
}