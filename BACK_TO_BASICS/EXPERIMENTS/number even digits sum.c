#include <stdio.h>

int even_digits_sum(int num) {
    if(num == 0)
        return 0;
    int sum = 0;
    if((num % 10) % 2 == 0)
        sum+= num % 10;
    sum += even_digits_sum(num / 10);
    return sum;
}

int main() {
    printf("%d\n", even_digits_sum(12356892));
}