/**	@file oper_exceptions.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif defines a few custom exceptions of operation exceptions
*/

#pragma once
#include <exception>
#include <string>
#include <sstream>

class BinOpException
{
public:
	BinOpException(std::string opert, std::string operOne, std::string operTwo);
};

class EmptyVariableOper : public std::exception
{
public:
	EmptyVariableOper(std::string oper);
};

// Exception class for mismatched operands and operators in a unary operation
class UnOpException : public std::exception
{
public:
	UnOpException(std::string opert, std::string oper);
};