#include <stdio.h>

int ft_isalnum(int c){
    return (c >= 48 & c <= 57) | (c >= 65 & c <= 90) | (c >= 97 & c <= 122);
}

int main() {
    printf("%d %d %d %d %d %d\n", ft_isalnum(47),ft_isalnum(48),ft_isalnum(64),ft_isalnum(65),ft_isalnum(96),ft_isalnum(97));
}