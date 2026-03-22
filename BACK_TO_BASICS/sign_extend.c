#include <stdio.h>
//sign extension,estende il segno di un numero a bits bit(ovviamente inferiore ad 8) al formato a 8 bit: in altre parole
// n e' un numero signed rappresentato solo da bits bit,estendilo a 8 bit mantenendo il segno
unsigned char sign_extend(unsigned char n, int bits) {
    return (n >> bits - 1 & 1) ? n | 255 << bits : n | 0 << bits;
}

int main() {
printf("%d\n",sign_extend(0b00000101, 4));
printf("%d\n",sign_extend(0b00001101, 4));
printf("%d\n",sign_extend(0b00000011, 3));
printf("%d\n",sign_extend(0b00000111, 3));
}