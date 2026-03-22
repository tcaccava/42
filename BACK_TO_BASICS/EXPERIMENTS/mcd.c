#include <stdio.h>

int mcd(int a, int b)//alla Euclide
{
    if (b == 0)
        return a;
    return mcd(b, a % b);
}


int main() {
    printf("%d\n", mcd(39,13));
}