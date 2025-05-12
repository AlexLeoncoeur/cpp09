#include "../../include/BitcoinExchange.hpp"

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
	int			separators = 0;
	size_t		pos = 0;

	pos = line.find(separator);
	if (pos == std::string::npos)
		return (NULL);
	while (pos != std::string::npos)
	{
		pos = line.find(separator, pos + 1);
		separators++;
	}
	pos = line.find(separator);
	switch (separators + 1)
	{
	case 2:
	{
		std::string **splitedLine = new std::string*[3];
		splitedLine[0] = trim(line.substr(0, pos), ' ');
		splitedLine[1] = trim(line.substr(pos + 1, line.size()), ' ');
		splitedLine[2] = nullptr;
		return (splitedLine);
	}
	case 3:
	{
		std::string **splitedLine = new std::string*[4];
		splitedLine[0] = trim(line.substr(0, pos), ' ');
		splitedLine[1] = trim(line.substr(pos + 1, line.find(separator, pos + 1)), ' ');
		splitedLine[2] = trim(line.substr(line.find(separator, pos + 1) + 1, line.size()), ' ');
		splitedLine[3] = nullptr;
		return (splitedLine);
	}
	default:
		return (NULL);
	}
}

static int	splitSize(std::string **splitedString)
{
	int i = 0;

	while (splitedString[i] != nullptr) 
		i++;
	return (i);
}

static std::multimap<std::string, std::string> fileToMultimap(std::ifstream &input)
{
	std::multimap<std::string, std::string>	data;
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
		for (int i = 0; splitedLine && splitedLine[i] != nullptr; i++)
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
		if (i == 0 && (input[i] == '-'))
			throw BitcoinExchange::dataError(SMALLNUMBER);
		if (!std::isdigit(input[i]) && input[i] != '.')
			throw BitcoinExchange::dataError(NOTNUMBER);
		if (input[i] == '.')
		{
			if (doutCount >= 1)
				throw BitcoinExchange::dataError(NOTNUMBER);
			doutCount++;
		}
	}
	if (std::atof(input.c_str()) > 1000)
		throw BitcoinExchange::dataError(BIGNUMBER);
	if (std::atof(input.c_str()) < 0)
		throw BitcoinExchange::dataError(SMALLNUMBER);
	if (std::atof(input.c_str()) > std::numeric_limits<float>::max())
		throw BitcoinExchange::dataError(DECIMALS);
}

static bool	isNum(std::string string)
{
	for (int i = 0; i < string.size(); i++)
	{
		if (!isdigit(string[i]))
			return (false);
	}
	return (true);
}

static void dateIsValid(const std::string input)
{
	std::istringstream			date(input);
	std::string					year, month, day;
	int							iYear, iMonth, iDay;
	char						delimiter = '-';
	bool						isValidDate = false;

	if (input.empty() || input.size() != 10)
		throw BitcoinExchange::dataError(INVALIDDATE);
	if (!(date >> delimiter >> year >> delimiter >> month >> delimiter >> day))
		throw BitcoinExchange::dataError(INVALIDDATE);
	if (year.size() != 4 || month.size() != 2 || day.size() != 2)
		throw BitcoinExchange::dataError(INVALIDDATE);
	if (!isNum(year) || !isNum(month) || !isNum(day))
		throw BitcoinExchange::dataError(INVALIDDATE);
	iYear = atoi(year.c_str());
	iMonth = atoi(month.c_str());
	iDay = atoi(day.c_str());
	switch(iMonth)
	{
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			isValidDate = (iDay >= 1 && iDay <= 31);
			break ;
		case 4: case 6: case 9: case 11:
			isValidDate = (iDay >= 1 && iDay <= 30);
			break ;
		case 2:
			isValidDate = (iYear % 4 == 0 && (iYear % 100 != 0 || iYear % 400 == 0) ? (iDay >= 1 && iDay <= 29) : (iDay >= 1 && iDay <= 28));
			break ;
	}
	if (!isValidDate)
		throw BitcoinExchange::dataError(INVALIDDATE);
}

/* static bool	checkDate(std::multimap<std::string, std::string>::iterator it)
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

static bool	checkValue(std::multimap<std::string, std::string>::iterator it)
{
	try
	{
		valueIsValid(it->second);
	}
	catch (std::exception &e)
	{
		std::cerr << RED << e.what() << it->second << RESET << std::endl;
		return (false);
	}
	return (true);
} */

BitcoinExchange::BitcoinExchange() : _inputFile()
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

BitcoinExchange::BitcoinExchange(const std::string inputRoute) : _inputFile(inputRoute)
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

BitcoinExchange::~BitcoinExchange()
{
	return ;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &rhs)
{
	if (this != &rhs)
	{
		this->_dataBase = rhs._dataBase;
		this->_inputFile = rhs._inputFile;
	}
	return (*this);
}

std::multimap<std::string, std::string>	BitcoinExchange::addDataBase(std::string dataBaseRoute)
{
	std::ifstream									input(dataBaseRoute.c_str());
	std::multimap<std::string, std::string>				aux;
	//std::multimap<std::string, std::string>::iterator	it;

	if (!input.is_open())
		throw invalidInput();
	aux = fileToMultimap(input);
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

static int	compareDates(std::string input, std::string base)//modificar
{
	std::string	**dividedDate;
	int			year;
	int			month;
	int			day;
	int			diff;

	dividedDate = split(input, '-');
	year = std::atoi(dividedDate[0]->c_str());
	month = std::atoi(dividedDate[1]->c_str());
	day = std::atoi(dividedDate[2]->c_str());
	for (int i = 0; dividedDate[i] != nullptr; i++)
		delete dividedDate[i];
	delete[] dividedDate;

	dividedDate = split(base, '-');
	year = year - std::atoi(dividedDate[0]->c_str());
	month = month - std::atoi(dividedDate[1]->c_str());
	day = day - std::atoi(dividedDate[2]->c_str());
	for (int i = 0; dividedDate[i] != nullptr; i++)
		delete dividedDate[i];
	delete[] dividedDate;

	diff = (year * 365) + (month * 30) + day;
	return (diff);
}

void	BitcoinExchange::outputResult(std::string line)//modificar
{
	std::istringstream					convertedLine(line);
	std::string							date, value;
	char								delimiter;
	double								res;

	std::multimap<std::string, std::string>::iterator	baseIt = this->_dataBase.begin();
	std::multimap<std::string, std::string>::iterator	closestDate = this->_dataBase.begin();

	convertedLine >> std::ws >> date >> std::ws >> delimiter >> std::ws >> value;
	for (; baseIt != this->_dataBase.end(); baseIt++)
	{
		int fCompare = compareDates(date, baseIt->first);
		int sCompare = compareDates(closestDate->first, baseIt->first);

		if (fCompare >= 0 && sCompare <= 0)
		{
			closestDate = baseIt;
			if (fCompare < 0 && sCompare < 0)
			break ;
			/* std::cout << BLUE << inputIt->first << RESET << std::endl;	
			std::cout << YELLOW << "compareDates("<< inputIt->first << ", "<< baseIt->first << ")" << " => " << "compareDates("<< closestDate->first << ", "<< baseIt->first << ")" << RESET << std::endl;	
			std::cout << WHITE << compareDates(inputIt->first, baseIt->first) << " => " << compareDates(closestDate->first, baseIt->first) << RESET << std::endl; */	
		}	
	}
	res = std::atof(value.c_str()) * std::atof(closestDate->second.c_str());
	std::cout << GREEN << date << " => " << value << " = " << res << RESET << std::endl;
}

static void	divideLine(std::string line)
{
	std::istringstream	convertedLine(line);
	std::string			date, value;
	char				delimiter;

	if (convertedLine >> std::ws >> date >> std::ws >> delimiter >> std::ws >> value)
	{
		size_t	pos = line.find(delimiter);
		if (isspace(line[pos + 2]) || isspace(line[pos - 2]) || !isspace(line[pos + 1]) || !isspace(line[pos - 1]))
			throw BitcoinExchange::dataError(INVALIDDATE);
		if (delimiter == '|' && convertedLine.eof() && line.find(date) == 0)
		{
			dateIsValid(date);
			valueIsValid(value);
		}
	}
}

void	BitcoinExchange::execute()
{
	bool								firstLine = true;
	std::string							readLine;
	std::ifstream						input(this->_inputFile);

	if (!input.is_open())
		throw invalidInput();
	while (std::getline(input, readLine))
	{
		try
		{
			divideLine(readLine);
		}
		catch (std::exception &e)
		{
			std::cerr << RED << e.what() << RESET << std::endl;
			continue ;
		}
		outputResult(readLine);
	}
}



/* 
	addInput sobra, hay que borrar _inputData y al comenzar execute crear un bucle que checkea la fecha y compara hasta encontrar el valor correcto.
*/