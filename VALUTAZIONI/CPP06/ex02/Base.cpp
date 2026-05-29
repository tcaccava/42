#include "Base.hpp"

Base* generate(void){
	srand((time(NULL)));
	int result = rand() % 3;

	switch (result) {
		case 0:
			std::cout << "A has been randomly generated" << std::endl;
			return new A;
			break;
		case 1:
			std::cout << "B has been randomly generated" << std::endl;
			return new B;
			break;
		case 2:
			std::cout << "C has been randomly generated" << std::endl;
			return new C;
			break;
	}
	return 0;
}

void identify(Base* p){
	if (dynamic_cast<const A*>(p))
		std::cout << "Type A" << std::endl;
	else if (dynamic_cast<const B*>(p))
		std::cout << "Type B" << std::endl;
	else if (dynamic_cast<const C*>(p))
		std::cout << "Type C" << std::endl;
	else
		std::cout << "p is not pointing to one of the classes" << std::endl;
}

void identify(Base& p){
	try
	{
		A& testA = dynamic_cast<A&>(p);
		std::cout<<"Type A"<<std::endl;
		(void)testA;
	}
	catch(const std::exception& e)
	{
		try
		{
			B& testB = dynamic_cast<B&>(p);
			std::cout<<"Type B"<<std::endl;
			(void)testB;
		}
		catch(const std::exception& e)
		{
			try
			{
				C& testC = dynamic_cast<C&>(p);
				std::cout<<"Type C"<<std::endl;
				(void)testC;
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
		}

	}
}

Base::~Base(){
}

A::A(){	
}

A::~A(){
	std::cout << "A destructor called" << std::endl;
}

B::B(){
}

B::~B(){
	std::cout << "B destructor called" << std::endl;
}

C::C(){
}

C::~C(){
	std::cout << "C destructor called" << std::endl;
}