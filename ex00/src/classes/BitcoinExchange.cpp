#include "../../include/BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() : _inputData()
{
	return ;
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &toCopy)
{
	if (this != &toCopy)
		*this = toCopy;
	return ;
}

BitcoinExchange::BitcoinExchange(const std::string inputRoute) : _inputData()
{
	try
	{
		this->addInput(inputRoute);
	}
	catch (std::exception &e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
		exit(1);
	}
	return ;
}

BitcoinExchange::~BitcoinExchange()
{
	return ;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &rhs)
{
	if (this != &rhs)
	{
		this->_dataBase = rhs._dataBase;
		this->_inputData = rhs._inputData;
	}
	return (*this);
}

void	BitcoinExchange::addInput(const std::string inputRoute)
{
	std::ifstream				input(inputRoute.c_str());
	std::map<std::string, int>	data;
	std::string					date;
	std::string					value;

	if (!input.is_open())
		throw invalidInput();
	data = fileToMap(input);
	
}

std::map<std::string, int> &fileToMap(const std::ifstream &input)
{
	std::string					readLine;
	bool						firstLine = true;
	char						separator;
	char						**splitedLine;

	while (std::getline(input, readLine))
	{
		if (firstLine)
		{
			firstLine = false;
			try
			{
				determineSeparator(readLine, separator);
			}
			catch (std::exception &e)
			{
				std::cerr << RED << e.what() << RESET << std::endl;
			}
			continue ;
		}
		splitedLine = split(readLine, separator);
		
	}
}

void	determineSeparator(const std::string &line, char &separator)
{
	std::string		commonSeparator = ",;|";

	for (int i = 0; i < commonSeparator.size(); i++)
		if (line.find(commonSeparator[i]) != std::string::npos)
			separator = commonSeparator[i];
	throw BitcoinExchange::noSeparator();
}

char	**split(const std::string &line, char separator)
{
	size_t	pos;

	pos = line.find(separator);
	
}
