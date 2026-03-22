#include <stdio.h>

int even_digit_sum(int *a, int size){
   if(size == 0)
    return 0;
    int result = 0;
    int tmp = (a[size - 1] % 2 == 0) ? a[size - 1] : 0;
    result = tmp + even_digit_sum(a,size - 1);
    return result;
}

int main() {
    int a[] = {1,2,3,4,5,6,7};
    printf("%d\n", even_digit_sum(a,7));
}