#include "../include/RPN.hpp"
#include <string>
#include <iostream>
#include <list>

int main(int argc, char **argv)
{
	if (argc != 2)
		return (std::cerr << RED << "Error: bad number of arguments." << RESET << std::endl, 1);
	RPN	operation(argv[1]);

	operation.doMath();
	return (0);
}