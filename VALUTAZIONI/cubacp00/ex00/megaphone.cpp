#include <iostream>



void    print_args(const char **av)
{
    int i, j;

    i = 0;
    while (av[++i])
    {
        j = -1;
        while (av[i][++j])
            std::cout << (char)std::toupper(av[i][j]);
        std::cout << " ";
    }
}

int main(int ac, const char **av)
{
    int i, j;

    if (ac == 1)
        std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *";
    else
        print_args(av);
    std::cout << std::endl;
    return (0);
}