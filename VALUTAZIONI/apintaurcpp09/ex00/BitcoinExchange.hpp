#ifndef BITCOIN_EXCHANGE_HPP
#define BITCOIN_EXCHANGE_HPP

#include <cstddef>
#include <map>
#include <string>

class BitcoinExchange {

  private:
	std::map<std::string, float> database;
	std::string					 filename;

	void validateHeader(std::string& header);
	void validateDate(const std::string& line, size_t& pos, std::string& date);
	void validateValue(const std::string& line, size_t pos, float& value);

  public:
	BitcoinExchange();
	BitcoinExchange(const std::string& filename);
	BitcoinExchange(const BitcoinExchange& b);

	BitcoinExchange& operator=(const BitcoinExchange& b);

	float getPriceByDate(const std::string& date) const;
	void  setNewPrice(const std::string& date, const float price);

	void load(const std::string& filename);
	void clear();
	void print();

	class BadFile : public std::exception {
	  public:
		virtual const char* what() const throw();
	};

	class BadData : public std::exception {
	  private:
		std::string message;

	  public:
		BadData(const std::string& l);
		virtual const char* what() const throw();
		virtual ~BadData() throw();
	};

	~BitcoinExchange();
};

void trim(std::string& s);

#endif
