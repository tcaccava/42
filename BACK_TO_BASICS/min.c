#include <stdio.h>

int ft_min(int a, int b)
{
    return (a < b) * a + (a >= b) * b;
}

// int ft_min(int a,int b){
//     return a ^ (a ^ b) & -(a >= b);
// }

int main()
{
    printf("%d %d %d %d\n", ft_min(128, 56), ft_min(255, 256), ft_min(1, 0), ft_min(20, 20));
}