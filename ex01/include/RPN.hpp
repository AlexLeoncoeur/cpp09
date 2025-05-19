#pragma once
# include <string>
# include <vector>

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

};
