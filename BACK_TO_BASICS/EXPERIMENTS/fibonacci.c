#include <stdio.h>

int fibonacci(int index)
{
    if(index == 0)
        return 0;
    if (index == 1)
        return 1;
    return fibonacci(index - 1) + fibonacci(index - 2);
}

int main() {
    printf("%d\n", fibonacci(6));
}