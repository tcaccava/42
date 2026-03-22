#include <stdio.h>
//converte un numero binario in BCD
unsigned char binary_to_bcd(unsigned char n) {
    if (n > 99)
        return 255;
    return (n / 10 << 4) | (n % 10);
}

int main() {
    printf("%d\n", 0x23);//23 diventa 00100011
}