#include <stdio.h>

int ft_max(int a, int b)
{
    return a ^ (a ^ b) & -(a <= b);
}

// int ft_max(int a,int b){
//     return (a > b) * a + (a <= b) * b;
// }

int main()
{
    printf("%d %d %d %d\n", ft_max(128, 56), ft_max(255, 256), ft_max(1, 0), ft_max(20, 20));
}