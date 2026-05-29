#include "ReversePolishNotation.hpp"

int main(int ac, char **av)
{
    if(ac != 2)
    {
        std::cout << "Usage: ./RPN [operations]" << std::endl;
        return 1;
    }

    ReversePolishNotation RPN(av[1]);
    RPN.calculate();
    
    return 0;
}
