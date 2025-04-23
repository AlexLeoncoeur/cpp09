#pragma once

# include <map>
# include <string>
# include <fstream>
# include <iostream>
# include "colors.hpp"

class BitcoinExchange
{

private:

	std::map<std::string, int>	_dataBase = addDataBase("../../data.csv");
	std::map<std::string, int>	_inputData;

	std::map<std::string, int>	addDataBase(std::string dataBaseRoute);

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
};

void	determineSeparator(const std::string &, char &);
char	**split(const std::string &, char);
std::map<std::string, int> &fileToMap(const std::ifstream &input);