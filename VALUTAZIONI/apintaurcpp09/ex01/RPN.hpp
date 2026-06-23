#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>

class Rpn {

  private:
	std::stack<int> stack;
	double			result;

	enum Operation { ADD, SUBTRACT, DIVIDE, MULTIPLY, OPERATION_COUNT };
	static const char* OPERATION_TOKENS[OPERATION_COUNT];

	Rpn(const Rpn& rpn);
	Rpn& operator=(const Rpn& rpn);
	void performOperation(const std::string& token);

  public:
	Rpn();

	const std::stack<int>& getStack() const;
	double				   getResult() const;

	void setStack(const std::stack<int>& s);
	void setResult(const double res);

	void compute(std::string& input);
	void reset();

	class CalculationFailed : public std::exception {
	  public:
		const char* what() const throw();
	};

	~Rpn();
};

#endif
