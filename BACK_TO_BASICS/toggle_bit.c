#include <stdio.h>

unsigned char toggle_bit(unsigned char n, int i){
    return (n ^ (1 << i));
}

int main() {
    int n = 15; //00001111
    int m = 13; //00001101
    printf("%d %d", toggle_bit(n, 1), toggle_bit(m,1));// 13 15
}