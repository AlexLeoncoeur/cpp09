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
	std::string					date;
	std::string					value;

	if (!input.is_open())
		throw invalidInput();
	this->_inputData = fileToMap(input);
}

std::map<std::string, std::string> &fileToMap(std::ifstream &input)
{
	std::map<std::string, std::string>	data;
	std::string					readLine;
	std::string					**splitedLine;
	bool						firstLine = true;
	char						separator;

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
		if (!splitedLine)
		{
			data.insert(std::make_pair(readLine, nullptr));
			continue ;
		}
		data.insert(std::make_pair(*splitedLine[0], *splitedLine[1]));
	}
	return (data);
}

void	determineSeparator(const std::string &line, char &separator)
{
	std::string		commonSeparator = ",;|";

	for (int i = 0; i < commonSeparator.size(); i++)
		if (line.find(commonSeparator[i]) != std::string::npos)
			separator = commonSeparator[i];
	throw BitcoinExchange::noSeparator();
}

std::string	trim(const std::string &line, char c)
{
	size_t	pos;
	size_t	lastPos;

	pos = line.find(c);
	lastPos = line.find_last_of(c);
	if (pos == std::string::npos)
		pos = 0;
	if (lastPos == std::string::npos)
		lastPos = line.size();
	return (line.substr(pos, lastPos));
}

std::string	**split(const std::string &line, char separator)
{
	size_t	pos;
	std::string *splitedLine[2];

	pos = line.find(separator);
	if (pos == std::string::npos)
		return (NULL);
	*splitedLine[0] = trim(line.substr(0, pos), ' ');
	*splitedLine[1] = trim(line.substr(pos, line.size()), ' ');
	return (splitedLine);
}
