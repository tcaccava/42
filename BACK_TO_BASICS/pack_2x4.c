#include <stdio.h>
//prende i niblle bassi di hi e lo e ne fa i nibble,rispettivamente alti e bassi,di un nuovo byte
unsigned char pack_2x4(unsigned char hi, unsigned char lo){
    return(hi << 4) | (lo & 15);
}

int main() {
    printf("%d\n", pack_2x4(0b00001010, 0b00000011));// 10100011,cioe' 163
}