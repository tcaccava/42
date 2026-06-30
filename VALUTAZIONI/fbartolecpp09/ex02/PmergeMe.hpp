#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <vector>
# include <deque>
# include <string>

// PmergeMe: sorts a sequence of positive integers with the
// merge-insertion algorithm (Ford-Johnson), using two different containers
// (std::vector and std::deque) to compare their performance.
class PmergeMe
{
	public:
		PmergeMe();
		PmergeMe(const PmergeMe& other);
		PmergeMe& operator=(const PmergeMe& other);
		~PmergeMe();

		// Reads the arguments (positive integers). Throws std::exception if invalid.
		void	parse(int argc, char** argv);
		// Runs the sort with both containers and prints the report.
		void	run();

	private:
		std::vector<int>	_vec;
		std::deque<int>		_deq;

		void	sortVector();
		void	sortDeque();
		void	printSequence(const std::string& label,
					const std::vector<int>& seq) const;
};

#endif
