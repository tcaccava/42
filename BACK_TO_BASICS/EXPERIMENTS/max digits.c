#include <stdio.h>

int max_digits(int num){
    if(num < 10)
        return num;
    int last_digit = num % 10;
    int max = max_digits(num / 10);
    return (last_digit > max) ? last_digit : max;
    
}

int main() {
    printf("%d\n", max_digits(22446688));
}