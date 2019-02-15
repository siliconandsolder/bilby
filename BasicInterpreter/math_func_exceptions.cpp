#include "math_func_exceptions.hpp"

/**
@name:		FuncException
@purpose:	Throws an exception for functions
@param:		std::string, std::string
@return:	FuncException
*/
FuncException::FuncException(std::string operation, std::string operand)
{
	std::ostringstream os;
	os << std::string("Cannot perform \"") << operation_ << std::string("\" function on a <") << operand_ << std::string(">");
	throw exception(os.str().c_str());
}

/**
@name:		EmptyVariableException
@purpose:	Throws an exception when an empty variable is found
@param:		std::string, std::string
@return:	EmptyVariableException
*/
EmptyVariableException::EmptyVariableException(std::string func)
{
	std::ostringstream os;
	os << std::string("Variable being used in a \"") << func_ << std::string("\" function is null");
	throw exception(os.str().c_str());
}
