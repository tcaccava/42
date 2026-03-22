#include <stdio.h>

int is_power_of_two(unsigned int n){
    return (n >0 && !(n & (n - 1)));
}

int main() {
    printf("%d\n", is_power_of_two(64));
    printf("%d\n", is_power_of_two(65));
    printf("%d\n", is_power_of_two(66));
}