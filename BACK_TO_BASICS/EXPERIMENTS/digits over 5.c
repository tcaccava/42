#include <stdio.h>

int digits_over_five(int num){
    if(num == 0)
        return 0;
    int occurrence = 0;;
    if((num % 10) > 5)
        occurrence++;
    occurrence += digits_over_five(num / 10);
    return occurrence;;
}

int main() {
    printf("%d\n", digits_over_five(44779911));
}