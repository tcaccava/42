#include "whatever.hpp"

int main()
{
	std::cout << min(4, 3) << std::endl;
	std::string one = "c";
	std::string two = "ciao";
	std::cout << ::min(two, one) << std::endl;

	std::string e = "ciao";
	std::string f = "hello";
	::swap(e, f);
	std::cout << e << std::endl;

	std::cout << max(21.05, 23.00) << std::endl;

	std::cout << "\nSUBJECT TESTS\n";
	int a = 2;
	int b = 3;
	::swap( a, b );
	std::cout << "a = " << a << ", b = " << b << std::endl;
	std::cout << "min( a, b ) = " << ::min( a, b ) << std::endl;
	std::cout << "max( a, b ) = " << ::max( a, b ) << std::endl;
	std::string c = "chaine1";
	std::string d = "chaine2";
	::swap(c, d);
	std::cout << "c = " << c << ", d = " << d << std::endl;
	std::cout << "min( c, d ) = " << ::min( c, d ) << std::endl;
	std::cout << "max( c, d ) = " << ::max( c, d ) << std::endl;
	return 0;
}
