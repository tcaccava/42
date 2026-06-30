#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <map>
# include <string>

// BitcoinExchange: loads a database (csv) "date,exchange_rate"
// into an std::map<date, rate> and evaluates an input file "date | value".
class BitcoinExchange
{
	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange& other);
		BitcoinExchange& operator=(const BitcoinExchange& other);
		~BitcoinExchange();

		// Loads the csv database. Throws std::runtime_error if not openable.
		void	loadDatabase(const std::string& filename);
		// Reads the input file line by line and prints the results.
		void	processInput(const std::string& filename);

	private:
		std::map<std::string, double>	_db;

		// Returns the rate valid at the given date (or the closest lower date).
		double	getRate(const std::string& date) const;
		// Validates that the date has format YYYY-MM-DD and is consistent.
		bool	isValidDate(const std::string& date) const;
		// Processes a single line of the input file.
		void	evaluateLine(const std::string& line) const;
};

#endif
