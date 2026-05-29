#include "Base.hpp"

int main(void){
	Base* randClass;

	randClass = generate();
	if (randClass == 0){
		std::cout << "Error in class generation" << std::endl;
		return 0;
	}
	identify(randClass);
	identify(randClass);
	delete randClass;
	return 0;
}