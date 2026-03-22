#include "easyfind.hpp"

int main() {
    std::vector<int> v;//empty container
    std::cout << "Empty : " << (v.empty() ? "yes" : "no") << std::endl;//returns true
    v.push_back(10);//add a member
    v.push_back(20);
    v.push_back(30);
    std::cout << "Empty : " << (v.empty() ? "yes" : "no") << std::endl;//returns false
    v.pop_back();//remove the last member
    std::cout << v.size() << std::endl;//returns container size
    for (int i = 0; i < v.size(); i++)
        std::cout << v.at(i) << std::endl;//returns index element,protected from out of bounds access
    try{
    std::cout << v.at(v.size() + 3) << std::endl;//out of bounds access
    }
    catch(const std::out_of_range &e){
        std::cerr << "Error : " << e.what()  << std::endl;}
    std::cout << v.front() << std::endl;//returns first member
    std::cout << v.back() << std::endl;//returns last member
    for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it)//returns iterator at beginning and over container's ending
        std::cout << *it << std::endl;
    std::vector<int>::iterator et = v.begin();
    v.insert(et, 42);//insert a new member before et position;
    std::cout << v.front() << std::endl;
    v.erase(v.begin());// erase the element from the given position
    v.assign(5, 52);//pushback n copies of a value in the container;
    v.resize(100);//increase or reduce the array size;if increse,new elements are zeroes
    v.clear();//removes all container's members;
    std::cout << "Empty : " << (v.empty() ? "yes" : "no")<< std::endl;
    std::list<int> x;
    x.push_back(5);
    x.push_back(10);
    x.push_back(15);
    try {
        std::list<int>::iterator it = easyfind(x, 3);
        std::cout << "Found: " << *it << std::endl;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
     try {
        std::list<int>::iterator it = easyfind(x, 15);
        std::cout << "Found: " << *it << std::endl; 
    }
    catch(std::exception &e){
        std::cerr << e.what() << std::endl;
    }
    std::deque<int> d;
    d.push_back(9);
    d.push_front(1);
    d.push_back(12);
    try{
    std::deque<int>::iterator it = easyfind(d,12);
    std::cout << "Found: " << *it << std::endl;
    }
    catch(std::exception &e){
        std::cerr << e.what() << std::endl;
    }
    std::set<int> set = {200, 100, 400};
    for (std::set<int>::iterator it = set.begin(); it != set.end(); it++)
        std::cout << *it << std::endl;
    try {
        std::set<int>::iterator it = easyfind(set, 200);
        std::cout << "Found: " << *it << std::endl; 
    }
    catch(std::exception &e){
        std::cerr << e.what() << std::endl;
    }
    std::set<int> wrongset = {1,1,2,3,3,4,3};
    for (std::set<int>::iterator it = wrongset.begin(); it != wrongset.end(); it++)
        std::cout << *it << std::endl;
}
