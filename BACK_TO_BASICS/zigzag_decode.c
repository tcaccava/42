#include <stdio.h>

signed char zigzag_decode(unsigned char n){
    return ((n ^ ( (n & 1) * 0xff)) >> 1) | (n << 7) ;
}

//oppure piu elegantemente
// signed char zigzag_decode(unsigned char n) {
//     return (n >> 1) ^ -(n & 1);
// }

int main()
{
    printf("%d\n", zigzag_decode(7));//-4
    printf("%d\n", zigzag_decode(3));//-2
    printf("%d\n", zigzag_decode(5));//-3
    printf("%d\n", zigzag_decode(4));// 2

}