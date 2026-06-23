#include "RPN.hpp"
#include <cstdlib>
#include <sstream>

const char* Rpn::OPERATION_TOKENS[OPERATION_COUNT] = {"+", "-", "/", "*"};

Rpn::Rpn() : stack(), result(0.0) {
}

const std::stack<int>& Rpn::getStack() const {
	if (stack.empty()) {
		throw CalculationFailed();
	} else {
		return (stack);
	}
}

double Rpn::getResult() const {
	if (stack.empty() || stack.size() != 1) {
		throw CalculationFailed();
	} else {
		return (result);
	}
}

void Rpn::performOperation(const std::string& token) {
	int opType = -1;
	for (int i = 0; i < OPERATION_COUNT; i++) {
		if (token == OPERATION_TOKENS[i]) {
			opType = i;
			break;
		}
	}

	if (opType == -1) {
		throw CalculationFailed();
	}

	int rstck = stack.top();
	stack.pop();
	int lstck = stack.top();
	stack.pop();

	int opResult = 0;
	switch (opType) {
	case ADD:
		opResult = lstck + rstck;
		break;
	case SUBTRACT:
		opResult = lstck - rstck;
		break;
	case MULTIPLY:
		opResult = lstck * rstck;
		break;
	case DIVIDE:
		if (rstck == 0) {
			throw CalculationFailed();
		}
		opResult = lstck / rstck;
		break;
	}
	stack.push(opResult);
}

void Rpn::compute(std::string& input) {
	// ./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"

	if (input.empty()) {
		throw CalculationFailed();
	}

	reset();

	std::istringstream iss(input);
	std::string		   token;

	while (iss >> token) {
		if (token.size() == 1 && token[0] >= '0' && token[0] <= '9') {
			stack.push(token[0] - '0');
		} else {
			if (stack.size() < 2) {
				throw CalculationFailed();
			}
			performOperation(token);
		}
	}

	if (stack.size() != 1) {
		throw CalculationFailed();
	}

	result = stack.top();
}

void Rpn::reset() {
	stack  = std::stack<int>();
	result = 0.0;
}

const char* Rpn::CalculationFailed::what() const throw() {
	return ("Error: Result calculation failed. Stack may be empty or invalid.");
}

Rpn::~Rpn() {
}
