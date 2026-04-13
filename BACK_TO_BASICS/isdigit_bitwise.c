#include <stdio.h>

int ft_isdigit(int c){
    return (c >= 48 & c <= 57);
}

int main() {
    printf("%d %d %d\n",  ft_isdigit(48), ft_isdigit(47), ft_isdigit(57));
}