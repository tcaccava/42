#include <stdio.h>

int num_digits_count(int num){
    int count = 0;
    if(num < 10)
        return 1;
    return 1 + num_digits_count(num / 10);    

    
}

int main() {
    printf("%d\n", num_digits_count(77889));
}