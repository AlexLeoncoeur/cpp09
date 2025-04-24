#include "../../include/BitcoinExchange.hpp"

static void	determineSeparator(const std::string &line, char &separator)
{
	std::string		commonSeparator = ",;|";

	for (int i = 0; i < commonSeparator.size(); i++)
		if (line.find(commonSeparator[i]) != std::string::npos)
		{
			separator = commonSeparator[i];
			return ;
		}
	throw BitcoinExchange::noSeparator();
}

static std::string	*trim(const std::string &line, char c)
{
	size_t	pos;
	size_t	lastPos;

	for (int i = 0; i <= line.size(); i++)
	{
		if (line[i] != c)
		{
			pos = i;
			break ;
		}
	}
	for (int i = line.size() - 1; i >= 0; i--)
	{
		if (line[i] != c)
		{
			lastPos = i;
			break ;
		}
	}
	std::string	*trimLine = new std::string;
	*trimLine = line.substr(pos, lastPos + 1);
	return (trimLine);
}

static std::string	**split(const std::string &line, char separator)
{
	int			separators = 1;
	size_t		pos = 0;

	pos = line.find(separator);
	if (pos == std::string::npos)
		return (NULL);
	while (pos == std::string::npos)
	{
		pos = line.find(separator, pos + 1);
		separators++;
	}
	switch (separators + 1)
	{
	case 2:
	{
		std::string **splitedLine = new std::string*[2];
		splitedLine[0] = trim(line.substr(0, pos), ' ');
		splitedLine[1] = trim(line.substr(pos + 1, line.size()), ' ');
		return (splitedLine);
	}
	case 3:
	{
		std::string **splitedLine = new std::string*[3];
		splitedLine[0] = trim(line.substr(0, pos), ' ');
		splitedLine[1] = trim(line.substr(pos + 1, line.find(separator, pos + 1)), ' ');
		splitedLine[2] = trim(line.substr(line.find(separator, pos + 1) + 1, line.size()), ' ');
		return (splitedLine);
	}
	default:
		return (NULL);
	}
}

static int	splitSize(std::string **splitedString)
{
	int i = 0;
	while (splitedString[i]) 
		i++;
	return (i);
}

static std::map<std::string, std::string> fileToMap(std::ifstream &input)
{
	std::map<std::string, std::string>	data;
	std::string							readLine;
	std::string							**splitedLine;
	bool								firstLine = true;
	char								separator;

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
				exit(1);
			}
			continue ;
		}
		splitedLine = split(readLine, separator);
		if (!splitedLine || splitSize(splitedLine) > 2)
			data.insert(std::make_pair(readLine, ""));
		else
			data.insert(std::make_pair(*splitedLine[0], *splitedLine[1]));
		for (int i = 0; splitedLine && splitedLine[i]; i++)
			delete splitedLine[i];
		delete[] splitedLine;
	}
	return (data);
}

static void valueIsValid(const std::string input)
{
	int	doutCount = 0;

	for (size_t i = 0; i < input.size(); i++)
	{
		if (i == 0 && (input[i] == '+'))
			continue ;
		if (!std::isdigit(input[i]) && input[i] != '.')
			throw BitcoinExchange::dataError(NOTNUMBER);
		if (input[i] == '.')
		{
			if (doutCount >= 1)
				throw BitcoinExchange::dataError(NOTNUMBER);
			doutCount++;
		}
	}
	if (std::atoi(input.c_str()) > 1000)
		throw BitcoinExchange::dataError(BIGNUMBER);
	if (std::atoi(input.c_str()) < 0)
		throw BitcoinExchange::dataError(SMALLNUMBER);
	if (std::atof(input.c_str()) > std::numeric_limits<float>::max())
		throw BitcoinExchange::dataError(DECIMALS);
}

static void DateIsValid(const std::string input)
{
	if (!input || input.size() != 10)
		throw BitcoinExchange::dataError(INVALIDDATE);
	for (int i = 0; i < input.size(); i++)
	{
		if ((i == 4 || i == 7) && input[i] == '-')
			continue ;
		if (!std::isdigit(input[i]))
			throw BitcoinExchange::dataError(INVALIDDATE);
		if ((i == 5) && (input[i] != '1' && input[i] != '0'))
			throw BitcoinExchange::dataError(INVALIDDATE);
		if ((i == 6) && (input[i - 1] == '1')
			&& (input[i] != '2' && input[i] != '1' && input[i] != '0'))
			throw BitcoinExchange::dataError(INVALIDDATE);
		if (i == 8 && (input[i] != '0' && input[i] != '1' && input[i] != '2' && input[i] != '3'))
			throw BitcoinExchange::dataError(INVALIDDATE);
		if (i == 9 && ((input[i] != '0' && input[i] != '1') && input[i - 1] == '3'))
			throw BitcoinExchange::dataError(INVALIDDATE);
	}
	std::cerr << CYAN << input << RESET << std::endl;
}

static bool	checkDate(std::map<std::string, std::string>::iterator it)
{
	try
	{
		DateIsValid(it->first);
	}
	catch (std::exception &e)
	{
		std::cerr << RED << e.what() << it->first << RESET << std::endl;
		return (false);
	}
	return (true);
}

static bool	checkValue(std::map<std::string, std::string>::iterator it)
{
	try
	{
		valueIsValid(it->second);
	}
	catch (std::exception &e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
		return (false);
	}
	return (true);
}

BitcoinExchange::BitcoinExchange() : _inputData()
{
	try
	{
		this->_dataBase = addDataBase("data.csv");
	}
	catch (std::exception &e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
		exit (1);
	}
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
		this->_dataBase = addDataBase("data.csv");
	}
	catch (std::exception &e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
		exit (1);
	}
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

std::map<std::string, std::string>	BitcoinExchange::addDataBase(std::string dataBaseRoute)
{
	std::ifstream									input(dataBaseRoute.c_str());
	std::map<std::string, std::string>				aux;
	//std::map<std::string, std::string>::iterator	it;

	if (!input.is_open())
		throw invalidInput();
	aux = fileToMap(input);
	/* it = aux.begin();
	for (; it != aux.end(); it++)
	{
		if (!checkDate(it) || !checkValue(it))
		{
			std::cerr << RED << "Error: bad database." << RESET << std::endl;
			exit(1);
		}		
	} */
	return (aux);
}

static int	compareDates(std::string input, std::string base)
{
	std::string	**dividedDate;
	int			year;
	int			month;
	int			day;
	int			diff;

	/* input 2020-05-14 base 2019-12-31 */
	/* input 1-7-17 base 2019-12-31 */
	/* input 365-210-17 base 2019-12-31 */
	/* input 138 base 2019-12-31 */
	/* input 2020-12-31 base 2021-01-01 */
	/* input -1 11 30 base 2021-01-01 */
	/* input -365 330 30 base 2021-01-01 */
	/* input 2021-01-01 base 2020-12-31 */
	/* input 1 -11 -30 base 2021-01-01 */
	/* input 365 -330 -30 base 2021-01-01 */
	/* input 2021-01-01 base 2020-12-31 */
	/* input 1 -11 -30 base 2021-01-01 */
	/* input 365 -330 -30 base 2021-01-01 */
	/* si es negativo nos hemos pasado y ya no es fecha valida */
	dividedDate = split(input, '-');
	year = std::atoi(dividedDate[0]->c_str());
	month = std::atoi(dividedDate[1]->c_str());
	std::cerr << WHITE << "." << **dividedDate << "." << RESET << std::endl;
	day = std::atoi(dividedDate[2]->c_str());
	delete[] dividedDate;

	dividedDate = split(base, '-');
	year = year - std::atoi(dividedDate[0]->c_str());
	month = month - std::atoi(dividedDate[1]->c_str());
	day = day - std::atoi(dividedDate[2]->c_str());
	delete[] dividedDate;

	diff = year + month + day;
	return (diff);
}

void	BitcoinExchange::execute()
{
	std::map<std::string, std::string>::iterator	inputIt = this->_inputData.begin();
	std::map<std::string, std::string>::iterator	baseIt = this->_dataBase.begin();
	std::map<std::string, std::string>::iterator	closestDate = this->_dataBase.begin();
	float											res;

	for (; inputIt != this->_inputData.end(); inputIt++)
	{
		std::cerr << GREEN << "." << inputIt->first << "." << RESET << std::endl;
		if (!checkDate(inputIt))
			continue ;
		std::cerr << BLUE << "." << inputIt->second << "." << RESET << std::endl;
		if (!checkValue(inputIt))
			continue ;
		for (; baseIt != this->_dataBase.end(); baseIt++)
		{
			std::cerr << MAGENTA << "." << baseIt->first << " " << inputIt->first << "." << RESET << std::endl;
			std::cerr << YELLOW << "." << (compareDates(inputIt->first, baseIt->first)) << "." << RESET << std::endl;
			if (compareDates(inputIt->first, baseIt->first) >= 0 && compareDates(baseIt->first, closestDate->first) < 0)
				closestDate = baseIt;
		}
		res = std::atof(inputIt->second.c_str()) * std::atof(closestDate->second.c_str());
		std::cout << GREEN << inputIt->first << " => " << inputIt->second << " = " << res << RESET << std::endl;
	}
}

void	BitcoinExchange::addInput(const std::string inputRoute)
{
	std::ifstream				input(inputRoute.c_str());

	if (!input.is_open())
		throw invalidInput();
	this->_inputData = fileToMap(input);
}
