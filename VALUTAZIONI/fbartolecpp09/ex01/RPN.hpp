#ifndef RPN_HPP
# define RPN_HPP

# include <string>
# include <stack>

// RPN: evaluates an expression in Reverse Polish Notation (postfix)
// using an std::stack as the operand stack.
class RPN
{
	public:
		RPN();
		RPN(const RPN& other);
		RPN& operator=(const RPN& other);
		~RPN();

		// Evaluates the expression. Throws std::runtime_error on error.
		int	evaluate(const std::string& expr);

	private:
		std::stack<int>	_stack;

		// Applies operator op between a and b (a op b).
		int	applyOp(int a, int b, char op) const;
};

#endif
