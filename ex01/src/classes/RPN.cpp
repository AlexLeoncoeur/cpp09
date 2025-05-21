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

void	RPN::outputResult(void)
{
	std::cout << YELLOW << this->_input << " = " << this->_result << RESET << std::endl;
}

static std::string	cleanWhiteSpace(std::string toClean)
{
	static std::istringstream	str(toClean);
	std::string					cleaned;
	std::string					c;

	if (str >> c)
	{
		if ((c.size() == 1 && (isdigit(c[0]) || c == "+" || c == "-" || c == "*" || c == "/")) || (c.size() == 2 && c[0] == '-' && isdigit(c[1])))
			cleaned += c;
		else	
			throw RPN::invalidInput();
		return (cleaned);
	}
	return ("");
}

static void	makeOperation(std::vector<float> &operands, char symbol)
{
	float	lhs, rhs;

	if (operands.size() < 2)
		throw RPN::cannotOperate(OPERATOR);
	lhs = operands[operands.size() - 2];
	rhs = operands[operands.size() - 1];

	operands.pop_back();
	operands.pop_back();

	if (symbol == '+')
		operands.push_back(lhs + rhs);
	else if (symbol == '-')
		operands.push_back(lhs - rhs);
	else if (symbol == '*')
		operands.push_back(lhs * rhs);
	else if (symbol == '/')
		operands.push_back(lhs / rhs);
}

void	RPN::doMath(void)
{
	std::string	nextTerm;

	try
	{
		while (1)
		{
			nextTerm = cleanWhiteSpace(this->_input);
			if (nextTerm.empty())
				break ;
			if (isdigit(nextTerm[0]) || (nextTerm[0] == '-' && isdigit(nextTerm[1])))
				this->_equation.push_back(atof(nextTerm.c_str()));
			else
				makeOperation(this->_equation, nextTerm[0]);
		}
		if (this->_equation.size() > 1)
			throw cannotOperate(OPERAND);
		this->_result = this->_equation[0];
		this->outputResult();
	}
	catch (std::exception &e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
		return ;
	}
}
