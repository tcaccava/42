#include <stdio.h>
int is_prime(unsigned int c)
{
    if(c < 2)
        return 0;
    if(c == 2)
        return 1;
    if (!(c & 1))
        return 0;
    int n = 3;
    while (n * n <= c)
    {
        if (!(c % n))
            return 0;
        n += 2;
    }
    return 1;
}

unsigned int next_prime(unsigned int n)
{
    while (!is_prime(n))
        n++;
    return n;
}

int main()
{
    printf("%d %d %d %d\n", is_prime(1), is_prime(57), is_prime(17), is_prime(29));
    printf("%d\n", next_prime(8));
}

// 1110 0010 0111 0000
// 1111 0110 0010 0011