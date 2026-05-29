#include "RPN.hpp"

RPN::RPN() {}

RPN::RPN(const RPN &src) {
	if (this != &src) {
		*this = src;
	}
}
RPN::RPN(std::string stack) {
	setStack(stack);
}

RPN &RPN::operator=(const RPN &src) {
	if (this != &src) {
		_stack = src._stack;
	}
	return *this;
}

RPN::~RPN() {
	while (!_stack.empty()) {
		_stack.pop();
	}
}

void RPN::setStack(std::string stack) {
	for (size_t i = 0; i < stack.length(); i++) {
		if (stack[i] == ' ')
			continue ;
		else if (ft_isDigit(stack[i])) {
			_stack.push(stack[i] - '0');
		}
		else if (ft_isOperator(stack[i])) {
			if (_stack.size() < 2)
				throw RPN::RPNError();
			double first = _stack.top();
			_stack.pop();
			double second = _stack.top();
			_stack.pop();
			double result;
			switch (stack[i]) {
				case '+' : result = second + first; break;
				case '-' : result = second - first; break;
				case '*' : result = second * first; break;
				case '/' : first == 0 ? throw RPN::RPNError() : result = second / first; break;
			}
			_stack.push(result);
		}
		else
			throw RPN::RPNError();
	}
}

void RPN::stackExec() {
	if (_stack.size() != 1)
		throw RPN::RPNError();
	std::cout << _stack.top() << std::endl;
}
