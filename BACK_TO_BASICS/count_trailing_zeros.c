#include <stdio.h>
//conta gli zeri consecutivi partendo dal bit piu' basso
unsigned char count_trailing_zeros(unsigned char n) {
    if(!n)
        return 8;
    int i = 0;
    for(; !(n >> i & 1); i++);
    return i;
}

int main()
{
    printf("%d\n",count_trailing_zeros(0b00001001));
    printf("%d\n",count_trailing_zeros(0b10000000));
    printf("%d\n",count_trailing_zeros(0b00000000));
    printf("%d\n",count_trailing_zeros(0b00000001));
}