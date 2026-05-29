# include <cstdlib>
# include <ctime>
# include "A.hpp"
# include "B.hpp"
# include "C.hpp"
# include "classIdentity.cpp"

int main() {
	srand(time(0));

	std::cout << "----- A -----" << std::endl;
	// A
	Base * a = new A;
	identify(a);
	identify(*a);
	delete a;

	std::cout << "----- B -----" << std::endl;

	// B
	Base * b = new B;
	identify(b);
	identify(*b);
	delete b;

	std::cout << "----- C -----" << std::endl;

	// C
	Base * c = new C;
	identify(c);
	identify(*c);
	delete c;

	std::cout << "----- Random -----" << std::endl;

	// random
	for (int i = 0; i < 5; i++) {
		Base * r1 = generate();
		identify(r1);
		identify(*r1);
		delete r1;
	}

	std::cout << "---------------" << std::endl;
}