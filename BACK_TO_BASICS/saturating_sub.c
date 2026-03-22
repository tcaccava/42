#include <stdio.h>

//se la sottrazione va in underflow ritorna 0 invece di andare in underflow
unsigned char saturating_sub(unsigned char a, unsigned char b){
    unsigned char sub = a - b;
    return sub & ((sub > a) - 1);
}

int main()
{
    printf("%d\n", saturating_sub(100, 50));
    printf("%d\n", saturating_sub(50, 100));
    printf("%d\n", saturating_sub(0, 1));
}