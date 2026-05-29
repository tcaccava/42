#ifndef RPN_HPP
# define RPN_HPP
#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <stdexcept>

template <typename T>
double ft_atod(const T &str) {
	std::istringstream iss(str);
	double num;
	iss >> num;
	if (iss.fail() || !iss.eof()) {
		throw std::invalid_argument("Invalid number format");
	}
	return num;
}

template <typename T>
bool ft_isDigit(const T &ch) {
	return ch >= '0' && ch <= '9';
}

template <typename T>
bool ft_isOperator(const T &ch) {
	return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

class RPN {
	private:
		std::stack<double> _stack;
	public:
		RPN();
		RPN(const RPN &src);
		RPN(std::string stack);
		RPN &operator=(const RPN &src);
		~RPN();
		void setStack(std::string stack);
		void stackExec();
		class RPNError : public std::exception {
			public:
				virtual const char* what() const throw() {
					return "Error";
				}
		};
};

#endif
