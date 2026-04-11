#include <stdio.h>

int toupper_bitwise(int c) {
    return c ^ ((c - 32 ^ c) & -(c >= 97 && c <= 122));
}

int main() {
    printf("%c %c\n", toupper_bitwise(97), toupper_bitwise(96));
}