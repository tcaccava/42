#include <stdio.h>

unsigned int ft_rotate_right32(unsigned int n, int k) {
    k %= 32;
    return (n >> k) | (n << 32 - k);
}

int main() {
    printf("%#x\n", ft_rotate_right32(0xFFaaFFaa,8));
}