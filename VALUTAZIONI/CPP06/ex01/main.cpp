#include "Serializer.hpp"
#include "Data.hpp"

int main(void){
	Data *test = new Data;
	Data *afterTest;
	uintptr_t uint;

	test->i = 1;
	test->str = "ciao";
	test->c = 'c';
	uint = Serializer::serialize(test);
	afterTest = Serializer::deserialize(uint);

	std::cout << "---------------------" << std::endl;
	std::cout << "Checking if data is ok...\n" << std::endl;
	std::cout << "Data int = " << test->i << std::endl;
	std::cout << "Data string = " << test->str << std::endl;
	std::cout << "Data char = " << test->c << std::endl;
	std::cout << "---------------------" << std::endl;
	std::cout << "Checking if pointers are equals...\n" << std::endl;
	if (test == afterTest){
		std::cout << test << " is equal to " << afterTest << ", GG!" << std::endl;
	}
	else{
		std::cout << "Please go find another job :)" << std::endl;
	}
	std::cout << "---------------------" << std::endl;
	delete test;
	return 0;
}