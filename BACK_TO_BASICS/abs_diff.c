#include <stdio.h>

unsigned char abs_diff(unsigned char a, unsigned char b){
    return (a - b) ^ (((a - b) ^ -(a - b)) & -(b > a));
}

// int main() {
//     printf("%d\n", abs_diff(5, 3));
//     printf("%d\n", abs_diff(3, 5));
//     printf("%d\n", abs_diff(3, 255));
//     printf("%d\n", abs_diff(255, 3));
// }