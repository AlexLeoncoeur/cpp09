#include "../../include/BitcoinExchange.hpp"

static std::multimap<std::string, float> fileToMultimap(std::ifstream &input)
{
	std::multimap<std::string, float>		data;
	std::string							readLine;
	std::string							**splitedLine;

	while (std::getline(input, readLine))
	{
		std::string				date;
		float					value;

		date = readLine.substr(0, 10);
		value = atof(readLine.substr(11).c_str());
		data.insert(std::make_pair(date, value));
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
	std::string					year, month, day;
	int							iYear, iMonth, iDay;
	bool						isValidDate = false;

	if (input.empty() || input.size() != 10)
		throw BitcoinExchange::dataError(INVALIDDATE);
	year = input.substr(0, 4);
	month = input.substr(5, 2);
	day = input.substr(8, 2);
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

std::multimap<std::string, float>	BitcoinExchange::addDataBase(std::string dataBaseRoute)
{
	std::ifstream									input(dataBaseRoute.c_str());
	std::multimap<std::string, float>				aux;

	if (!input.is_open())
		throw invalidInput();
	aux = fileToMultimap(input);
	return (aux);
}

static int	convertToDays(int year, int month)
{
	int		res = 0;

	switch(month)
	{
		case 12:
			res += 31;
		case 11:
			res += 30;
		case 10:
			res += 31;
		case 9:
			res += 30;
		case 8:
			res += 31;
		case 7:
			res += 31;
		case 6:
			res += 30;
		case 5:
			res += 31;
		case 4:
			res += 30;
		case 3:
			res += 31;
		case 2:
			(year % 4 == 0 && (year % 100 != 0 || year % 400 == 0) ? (res += 29) : (res += 28));
		case 1:
			res += 31;
	}
	return (res);
}

static int	compareDates(std::string input, std::string base)
{
	std::istringstream	fullDate(input);
	int					iYear, iMonth, iDay;
	int					bYear, bMonth, bDay;
	char				delimiter;

	fullDate >> iYear >> delimiter >> iMonth >> delimiter >> iDay;
	fullDate.str(base);
	fullDate.clear();
	fullDate.seekg(0);
	fullDate >> bYear >> delimiter >> bMonth >> delimiter >> bDay;
	

	return (((iYear - bYear) * 365) + ((convertToDays(iYear, iMonth) - convertToDays(bYear, bMonth))) + (iDay - bDay));
}

void	BitcoinExchange::outputResult(std::string line)
{
	std::istringstream					convertedLine(line);
	std::string							date;
	float								value;
	char								delimiter;
	double								res;

	std::multimap<std::string, float>::iterator	baseIt = this->_dataBase.begin();
	std::multimap<std::string, float>::iterator	closestDate = this->_dataBase.begin();

	convertedLine >> std::ws >> date >> std::ws >> delimiter >> std::ws >> value;
	for (; baseIt != this->_dataBase.end(); baseIt++)
	{
		int firstCompare = compareDates(date, baseIt->first);
		int secondCompare = compareDates(closestDate->first, baseIt->first);

		if (firstCompare >= 0 && secondCompare <= 0)
		{
			closestDate = baseIt;
			if (firstCompare < 0 && secondCompare < 0)
				break ;
		}	
	}
	res = value * closestDate->second;
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
		return ;
	}
	throw BitcoinExchange::dataError(INVALIDDATE);
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
			if (firstLine)
			{
				firstLine = false;
				continue ;
			}
			divideLine(readLine);
		}
		catch (std::exception &e)
		{
				std::cerr << RED << e.what() << readLine << RESET << std::endl;
			continue ;
		}
		outputResult(readLine);
	}
}
