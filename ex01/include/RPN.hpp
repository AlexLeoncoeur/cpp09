#pragma once

# include <string>
# include <vector>
# include <sstream>
# include <iostream>
# include "colors.hpp"

enum errorType
{
	OPERATOR,
	OPERAND,
};

class RPN
{

private:

	float				_result;
	std::string			_input;
	std::vector<float>	_equation;

public:

	RPN();
	RPN(const RPN &);
	RPN(const std::string _input);
	~RPN();

	RPN	&operator=(const RPN &);

	void	outputResult(void);
	void	doMath(void);

	class invalidInput : public std::exception
	{
		virtual const char* what() const throw()
		{
			return ("error: invalid input");
		}
	};

	class cannotOperate : public std::exception
	{
		private:
			errorType	_error;
		public:
			cannotOperate(errorType err) : _error(err) {};

			virtual const char* what() const throw()
			{
				switch (_error)
				{
					case (OPERATOR):
						return ("error: not enough operands for operator");
						break ;
					case (OPERAND):
						return ("error: not enough operators for operands");
						break ;
					default :
						return ("error: unknown");
				}
			}
	};
};
