#include <stdio.h>

int ft_isascii(int c){
    return !(c >> 7 & 1);
}
// oppure ancora più elegante
// return !(c & ~127);

int main() {
    printf("%d %d %d %d\n", ft_isascii(42), ft_isascii(127), ft_isascii(128), ft_isascii(0));
}