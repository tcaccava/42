#include "RPN.hpp"
#include <stack>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <exception>

int calc(const char *expr)
{
    std::stack<int> st;

    for(;*expr;expr++)
    {
        if(isdigit(*expr))
        {
            st.push(*expr - '0');
            continue;
        }
        if(isspace(*expr))
            continue;
        if(st.size() < 2)
        {
            throw std::runtime_error("Malformed expression, not enough operands"
                "to execute the operation\n");
        }
        int b = st.top();st.pop();
        int a = st.top();st.pop();
        switch(*expr)
        {
            case '+':st.push(a + b);break;
            case '-':st.push(a - b);break;
            case '*':st.push(a * b);break;
            case '/':st.push(a / b);break;
            default:
                throw std::runtime_error((std::string("Unsupported token:") + *expr).c_str());
        }
    }
    if(st.size() > 1)
    {
        throw std::runtime_error("Malformed operation, #operands > #operators-1\n");
    }
    return st.top();
}
