#include <stdio.h>

unsigned int ft_rotate_left32(unsigned int n, int k) {
    return (n << k) | (n >> 32 - k);
}

int main() {
    printf("%u\n", ft_rotate_left32(0b00000000111111110000000011111111, 8));
    printf("%#x\n", ft_rotate_left32(0x00FF00FF, 8));
}

