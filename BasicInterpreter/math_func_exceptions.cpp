/**	@file math_func.cpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of math function exceptions
*/

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
