#include <stdio.h>

int are_digits_all_even(int num){
    if(num < 10 && num % 2 == 0)
        return 1;
    int last_digit = num % 10;
    if(last_digit % 2 != 0)
        return 0;
    return are_digits_all_even(num / 10);
}

int main() {
    printf("%d\n", are_digits_all_even(2244));
    printf("%d\n", are_digits_all_even(2243));
    printf("%d\n", are_digits_all_even(22445));
    printf("%d\n", are_digits_all_even(224466));

}