#include "PmergeMe.hpp"
#include <iostream>
#include <string>
#include <ctime>
#include <algorithm>

using std::cerr;
using std::string;
using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    if(argc <= 1)
    {
        cout << "Usage: " << argv[0] << " <list of positive integers>"<<endl;
        return 1;
    }
    
    string s = " ";
    for (int i = 1; i < argc; i++)
    {
        s += argv[i];
        s += " ";
    }

    std::vector<unsigned int> original;
    try
    {
        original = parse_vec(s);
    }
    catch (std::runtime_error &e)
    {
        cerr << e.what() << endl;
        return 1;
    }
    // vector part begins here
    // I'll make a copy to have consistent times across vector and deque
    std::vector<unsigned int> vec(original.begin(), original.end()); 
    clock_t clk = clock(); 
    
    merge_insert_sort(vec);
    clk = clock() - clk;
    cout << "Before: ";
    for(size_t i = 0; i < original.size(); i++)
    {
        cout << original[i] << ' ';
    }
    cout << "\n";

    cout << "After: ";
    for(size_t i = 0; i < original.size(); i++)
    {
        cout << vec[i] << ' ';
    }
    cout << "\n";
    cout << "time for std::vector<unsigned int >= " << (clk * 1000000) / CLOCKS_PER_SEC << " us " << endl;
    
    
    // deque part begins here
    std::deque<unsigned int> dq(original.begin(), original.end());
    clk = clock();
    merge_insert_sort(dq);
    clk = clock() - clk;
    cout << "time for std::deque<unsigned int> = " << (clk * 1000000) / CLOCKS_PER_SEC << " us " << endl;
    

}
