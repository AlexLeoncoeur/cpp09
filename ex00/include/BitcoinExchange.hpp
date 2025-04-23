#pragma once

# include <map>
# include <string>
# include <fstream>
# include <iostream>
# include <limits>
# include "colors.hpp"

enum errorType
{
	BIGNUMBER,
	SMALLNUMBER,
	NOTNUMBER,
	DECIMALS,
	INVALIDDATE,
	VALID,
};

class BitcoinExchange
{

private:

	std::map<std::string, std::string>	_inputData;
	std::map<std::string, std::string>	_dataBase;

	std::map<std::string, std::string>	addDataBase(std::string dataBaseRoute);

public:

	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &);
	BitcoinExchange(const std::string);
	~BitcoinExchange();

	BitcoinExchange &operator=(const BitcoinExchange &);

	void	addInput(const std::string);
	void	execute();

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
					return ("Error: number greater than 1000.");
					break;
				case (SMALLNUMBER):
					return ("Error: not a positive number.");
					break;
				case (NOTNUMBER):
					return ("Error: not a number.");
					break;
				case (DECIMALS):
					return ("Error: number greater than float.");
					break;
				case (INVALIDDATE):
					return ("Error: bad input => ");
					break;				
				default:
					return ("Error: unspecified error.");
					break;
				}
			}
	};
};

/* void								determineSeparator(const std::string &, char &);
std::string							**split(const std::string &, char);
std::string							trim(const std::string &, char);
std::map<std::string, std::string>	&fileToMap(std::ifstream &input); */