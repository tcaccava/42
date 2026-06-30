#include "RPN.hpp"
#include <sstream>
#include <stdexcept>
#include <cctype>

RPN::RPN() {}

RPN::RPN(const RPN& other) : _stack(other._stack) {}

RPN& RPN::operator=(const RPN& other)
{
	if (this != &other)
		_stack = other._stack;
	return (*this);
}

RPN::~RPN() {}

int	RPN::applyOp(int a, int b, char op) const
{
	switch (op)
	{
		case '+': return (a + b);
		case '-': return (a - b);
		case '*': return (a * b);
		case '/':
			if (b == 0)
				throw std::runtime_error("division by zero");
			return (a / b);
		default:
			throw std::runtime_error("unknown operator");
	}
}

int	RPN::evaluate(const std::string& expr)
{
	std::istringstream	iss(expr);
	std::string			token;

	while (iss >> token)
	{
		if (token.size() != 1)
			throw std::runtime_error("invalid token");
		char	c = token[0];
		if (std::isdigit(static_cast<unsigned char>(c)))
		{
			// The given numbers are always < 10, so a single digit.
			_stack.push(c - '0');
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/')
		{
			// Two operands on the stack are needed to apply an operator.
			if (_stack.size() < 2)
				throw std::runtime_error("not enough operands");
			int	b = _stack.top(); _stack.pop();
			int	a = _stack.top(); _stack.pop();
			_stack.push(applyOp(a, b, c));
		}
		else
			throw std::runtime_error("invalid token");
	}
	// At the end of the expression exactly one value must remain.
	if (_stack.size() != 1)
		throw std::runtime_error("invalid expression");
	return (_stack.top());
}
