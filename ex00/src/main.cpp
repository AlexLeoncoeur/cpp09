#include "../include/BitcoinExchange.hpp"
#include <string>
#include <iostream>
#include <list>

int main(int argc, char **argv)
{
	if (argc != 2)
		return (std::cerr << RED << "Error: bad number of arguments." << RESET << std::endl, 1);
	try
	{
		BitcoinExchange	exchange(argv[1]);
		exchange.execute();
	}
	catch (std::exception &e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	return (0);
}