#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <ctime>
#include <list>
#include <vector>
#include <string>

class PmergeMe {

  private:
	std::vector<int> vec;
	std::list<int>	 lst;
	double			 vecTime;
	double			 listTime;

  public:
	PmergeMe();
	PmergeMe(const std::string& input);
	PmergeMe(const PmergeMe& p);

	PmergeMe& operator=(const PmergeMe& p);

	inline const std::vector<int>& getVector() const { return (vec); };
	inline const std::list<int>&   getList() const { return (lst); };
	inline double				   getVecTime() const { return (vecTime); };
	inline double				   getListTime() const { return (listTime); };

	void setVector(const std::vector<int>& v);
	void setList(const std::list<int>& l);

	void sortVector();
	void sortList();

	void printVector();
	void printList();
	void reset();

	~PmergeMe();
};

#endif
