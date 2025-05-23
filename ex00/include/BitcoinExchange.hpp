#pragma once

# include <map>
# include <string>
# include <fstream>
# include <iostream>
# include <sstream>
# include <limits>
# include "colors.hpp"

enum errorType
{
	BIGNUMBER,
	SMALLNUMBER,
	NOTNUMBER,
	DECIMALS,
	INVALIDDATE,
	DELIMITER,
	VALID,
};

class BitcoinExchange
{

private:

	std::string							_inputFile;
	std::multimap<std::string, float>	_dataBase;

	std::multimap<std::string, float>	addDataBase(std::string dataBaseRoute);

public:

	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &);
	BitcoinExchange(const std::string);
	~BitcoinExchange();

	BitcoinExchange &operator=(const BitcoinExchange &);

	void	execute();
	void	outputResult(std::string);

	class invalidInput : public std::exception
	{
		public:
			const char* what() const throw()
			{
				return ("Error: invalid input route or name");
			}
	};

	class noSeparator : public std::exception
	{
		public:
			const char* what() const throw()
			{
				return ("Error: invalid or unknown separator in file header");
			}
	};

	class dataError : public std::exception
	{
		private:
			
			errorType	error;

		public:

			dataError(errorType err) : error(err) {};

			virtual const char* what() const throw()
			{
				switch (error)
				{
				case (BIGNUMBER):
					return ("Error: number greater than 1000 => ");
					break;
				case (SMALLNUMBER):
					return ("Error: not a positive number => ");
					break;
				case (NOTNUMBER):
					return ("Error: not a number => ");
					break;
				case (DECIMALS):
					return ("Error: number greater than float => ");
					break;
				case (INVALIDDATE):
					return ("Error: bad input => ");
					break;
				case (DELIMITER):
					return ("Error: bad delimeter => ");
					break;				
				default:
					return ("Error: unspecified error => ");
					break;
				}
			}
	};
};

/* void								determineSeparator(const std::string &, char &);
std::string							**split(const std::string &, char);
std::string							trim(const std::string &, char);
std::map<std::string, std::string>	&fileToMap(std::ifstream &input); */