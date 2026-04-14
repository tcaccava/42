#include <stdio.h>
//conta i bit accesi consecutivi partendo dall' LSB
int ft_count_trailing_ones(unsigned char n) {
    int i = 0;
    for(; n >> i & 1; i++);
    return i;
}

int main() {
    printf("%d\n", ft_count_trailing_ones(0b01010111));
    printf("%d\n", ft_count_trailing_ones(0b01010100));
    printf("%d\n", ft_count_trailing_ones(0b11111111));
}