#include <stdio.h>

int ft_sign(int n){
    return -1 * ((n >> 31) & 1) + 1 * (n > 0);
}
//oppure illegibile
// return -(n >> 31) | !!n;
int main() {
    printf("%d %d %d\n", ft_sign(1), ft_sign(-1), ft_sign(0));
}