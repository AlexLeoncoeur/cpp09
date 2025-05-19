#include "../../include/RPN.hpp"

RPN::RPN() : _result(0), _input("NoInput"), _equation()
{
}

RPN::RPN(const RPN & toCopy)
{
	if (this != &toCopy)
		*this = toCopy;
}

RPN::RPN(const std::string input) : _result(0), _input(input), _equation()
{
}

RPN::~RPN()
{
}

RPN	&RPN::operator=(const RPN &rhs)
{
	if (this != &rhs)
	{
		this->_result = rhs._result;
		this->_input = rhs._input;
		this->_equation = rhs._equation;
	}
	return (*this);
}

void	RPN::doMath(void)
{
	//return string without ws
	//read string, check current data, parse, add to vector
	//check if operation is available and do if so
	//if no more string check final content of vector, error or output
}
