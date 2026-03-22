#ifndef RPN_HPP
#define RPN_HPP

#include <string>
#include <iostream>
#include <stack>
#include <fstream>
#include <sstream>

class RPN {
    public:
    RPN();
    ~RPN();
    RPN(const RPN& other);
    RPN &operator=(const RPN &other);

    int do_op(int a, int b, char signum);
    void create_stack(const char *str);
    bool is_op(const char op);
    bool check_error(const char *str);


    private:
        std::stack<int> array;
};

#endif