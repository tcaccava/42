#include <unistd.h>

void print_down(int n)
{
    if (n == 0)
    {
        write(1, "\n", 1);
        return;
    }
    char c = n + '0';
    write(1, &c, 1);
    print_down(n - 1);
}

int main()
{
    print_down(5);
}