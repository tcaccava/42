#include "ReversePolishNotation.hpp"
#include <exception>

ReversePolishNotation::ReversePolishNotation(std::string operations)
{
	this->operations = operations;
}

ReversePolishNotation::~ReversePolishNotation()
{
}

ReversePolishNotation::ReversePolishNotation(const ReversePolishNotation &other)
{
	//*this = other;
	this->operations = other.operations;
}

ReversePolishNotation &ReversePolishNotation::operator=(const ReversePolishNotation &other)
{
	this->operations = other.operations;
	return *this;
}

ReversePolishNotation::ReversePolishNotation()
{
}

bool check_value_validity(std::string value)
{
	int num = atoi(value.c_str());
	if(num < 0 || num >= 10)
		return 0;
	if(value.find_first_not_of("0123456789") != std::string::npos)
		return 0;
	return 1;
}

bool is_operation(std::string token)
{
	if(token == "+" || token == "-" || token == "*" || token == "/")
		return true;
	return false;
}

void ReversePolishNotation::calculate()
{
	std::stack<int> stack;

	std::stringstream ss(this->operations);
	std::string token;
	try {
		while (std::getline(ss, token, ' ')) {
			if (token.empty())
		continue;
			if(is_operation(token))
			{
				if(stack.size() < 2)
					throw std::exception();
				int a = stack.top();
				stack.pop();
				int b = stack.top();
				stack.pop();
				if(token == "+")
					stack.push(b + a);
				else if(token == "-")
					stack.push(b - a);
				else if(token == "*")
					stack.push(b * a);
				else if(token == "/")
				{
					if(a == 0)
						throw std::exception();
					stack.push(b / a);
				}
			}
			else {
				if(!check_value_validity(token))
					throw std::exception();
				stack.push(atoi(token.c_str()));
			}
		}
		if(stack.size() != 1)
			throw std::exception();
		std::cout << stack.top() << std::endl;
	}
	catch (std::exception &e) {std::cerr << "Error" << std::endl; exit(1);}

}