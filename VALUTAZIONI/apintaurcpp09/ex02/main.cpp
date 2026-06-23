#include "PmergeMe.hpp"
#include <iostream>

int main(int argc, char* argv[]) {

	if (argc < 2) {
		std::cerr << "Error: invalid arguments" << std::endl;
		return (-1);
	}

	try {
		int			i = 1;
		std::string input;
		while (argv && (*((argv) + i))) {
			input += argv[i++];
			input += " ";
		}
		PmergeMe p(input);

		std::cout << "Before:\t";
		p.printVector();
		std::cout << std::endl;
		std::cout << "After:\t";
		p.sortVector();
		p.sortList();
		p.printVector();
		std::cout << std::endl;

		std::cout << "Time to process a range of " << sizeof(p.getVector())
				  << " elements with std::vector : " << p.getVecTime() << " us"
				  << std::endl;
		std::cout << "Time to process a range of " << sizeof(p.getVector())
				  << " elements with std::list : " << p.getListTime() << " us"
				  << std::endl;
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

// Switch comments for automatic tests
// #include "PmergeMe.hpp"
// #include <cstdlib>
// #include <ctime>
// #include <iostream>
// #include <limits>
// #include <string>
// #include <vector>

// void runTest(const std::vector<std::string>& args,
// 			 const std::string&				 description) {
// 	std::cout << "\n=== Test: " << description << " ===" << std::endl;

// 	int				   argc = args.size() + 1;
// 	std::vector<char*> argv(argc);
// 	argv[0] = (char*)"PmergeMe";
// 	for (size_t i = 0; i < args.size(); ++i) {
// 		argv[i + 1] = const_cast<char*>(args[i].c_str());
// 	}

// 	std::string input;
// 	for (size_t i = 1; i < argv.size(); ++i) {
// 		input += argv[i];
// 		input += " ";
// 	}

// 	try {
// 		PmergeMe p(input);

// 		std::cout << "Before:\t";
// 		p.printVector();
// 		std::cout << std::endl;

// 		p.sortVector();
// 		p.sortList();

// 		std::cout << "After:\t";
// 		p.printVector();
// 		std::cout << std::endl;

// 		std::cout << "Time to process a range of " << p.getVector().size()
// 				  << " elements with std::vector : " << p.getVecTime() << " us"
// 				  << std::endl;
// 		std::cout << "Time to process a range of " << p.getVector().size()
// 				  << " elements with std::list : " << p.getListTime() << " us"
// 				  << std::endl;
// 	} catch (const std::exception& e) {
// 		std::cerr << "Exception: " << e.what() << std::endl;
// 	}
// }

// int main() {
// 	std::srand(static_cast<unsigned int>(std::time(NULL)));

// 	std::vector<std::string> test1;
// 	test1.push_back("3"); test1.push_back("1"); test1.push_back("2"); test1.push_back("5"); test1.push_back("4");
// 	runTest(test1, "Small sequential numbers");

// 	std::vector<std::string> test2;
// 	test2.push_back("5"); test2.push_back("3"); test2.push_back("3"); test2.push_back("2"); test2.push_back("5"); test2.push_back("1");
// 	runTest(test2, "Duplicates");

// 	std::vector<std::string> test3;
// 	test3.push_back("-2"); test3.push_back("-5"); test3.push_back("0"); test3.push_back("3"); test3.push_back("-1");
// 	runTest(test3, "Negative numbers");

// 	std::vector<std::string> test4;
// 	test4.push_back("2147483647"); test4.push_back("0"); test4.push_back("-2147483648"); test4.push_back("1000000");
// 	runTest(test4, "Large integer values");

// 	std::vector<std::string> test5;
// 	test5.push_back("1"); test5.push_back("2"); test5.push_back("3"); test5.push_back("4"); test5.push_back("5"); test5.push_back("6");
// 	runTest(test5, "Already sorted");

// 	std::vector<std::string> test6;
// 	test6.push_back("6"); test6.push_back("5"); test6.push_back("4"); test6.push_back("3"); test6.push_back("2"); test6.push_back("1");
// 	runTest(test6, "Reverse sorted");

// 	std::vector<std::string> test7;
// 	test7.push_back("42");
// 	runTest(test7, "Single element");

// 	std::vector<std::string> test8;
// 	runTest(test8, "Empty input");
// 	// Stress test random
// 	int sizes[] = {10, 50, 100, 500, 1000, 5000, 10000};
// 	for (unsigned long idx = 0; idx < sizeof(sizes)/sizeof(sizes[0]); ++idx) {
// 		int size = sizes[idx];
// 		std::vector<std::string> randomInput;
// 		for (int i = 0; i < size; ++i) {
// 			randomInput.push_back(std::to_string(std::rand() % 10000));
// 		}
// 		runTest(randomInput,
// 				"Random " + std::to_string(size) + " positive elements");
// 	}

// 	// Test sequenze monotone lunghe
// 	std::vector<std::string> ascending;
// 	for (int i = 0; i < 100; ++i)
// 		ascending.push_back(std::to_string(i));
// 	runTest(ascending, "Ascending 0-99");

// 	std::vector<std::string> descending;
// 	for (int i = 99; i >= 0; --i)
// 		descending.push_back(std::to_string(i));
// 	runTest(descending, "Descending 99-0");

// 	// Test sequenze ripetute e pattern
// 	std::vector<std::string> repeated;
// 	for (int i = 0; i < 50; ++i)
// 		repeated.push_back("7");
// 	runTest(repeated, "Repeated same value 7");

// 	std::vector<std::string> pattern;
// 	for (int i = 0; i < 50; ++i)
// 		pattern.push_back(std::to_string((i % 5) + 1));
// 	runTest(pattern, "Pattern 1-5 repeated");

// 	// Test valori limite
// 	std::vector<std::string> limits;
// 	limits.push_back("0");
// 	limits.push_back("1");
// 	limits.push_back("2147483647");
// 	limits.push_back("2147483646");
// 	runTest(limits, "Edge values 0, 1, INT_MAX");

// 	// Test input misti
// 	std::vector<std::string> mixed;
// 	for (int i = -10; i <= 10; ++i)
// 		mixed.push_back(std::to_string(i));
// 	runTest(mixed, "Mixed negative to positive numbers");

// 	return 0;
// }
