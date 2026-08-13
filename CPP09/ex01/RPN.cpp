#include "RPN.hpp"

RPN::RPN() {}

RPN::~RPN() {}

RPN::RPN(const RPN &other) : array(other.array) {}

RPN &RPN::operator=(const RPN &other)
{
    if (this != &other)
        array = other.array;
    return *this;
}

int RPN::do_op(int a, int b, char signum)
{
    if (signum == '+')
        return a + b;
    else if (signum == '-')
        return a - b;
    else if (signum == '*')
        return a * b;
    else
    {
        if (b == 0)
            std::cerr << "Error: division by zero" << std::endl;
        return a / b;
    }
}

bool RPN::is_op(const char op)
{
    return (op == '+' || op == '-' || op == '*'|| op == '/');
}

bool RPN::check_error(const char *str)
{
    std::string input = str;
    std::istringstream iss(input);
    std::string token;
    while(iss >> token){
        if(token.length() == 1)
        {
            if(isdigit(token[0]))
                continue;
            else if(is_op(token[0]))
                continue;
        }
        return true;
    }
    return false;
    }
    // std::string string = str;
    // for (int i = 0; i < string.length(); i++)
    // {
    //     if ((!isdigit(string[i]) && !is_op(string[i]) && string[i] != ' '))
    //         return true;
    // }
    // return false;


void RPN::create_stack(const char *str)
{
    std::string argument = str;
    int result = 0;
    for (int i = 0; i < argument.length(); i++)
    {
        if (i % 2 == 1)
            continue;
        if (isdigit(argument[i]))
            array.push((argument[i] - '0'));
        if (is_op(argument[i]))
        {
            if (array.size() < 2)
            {
                std::cerr << "Error" << std::endl;
                return;
            }
            int front = array.top();
            array.pop();
            int front2 = array.top();
            array.pop();
            result = do_op(front2, front, argument[i]);
            array.push(result);
        }
    }
    if (array.size() != 1)
    {
        std::cerr << "Error" << std::endl;
        return;
    }
    std::cout << result << std::endl;
}
