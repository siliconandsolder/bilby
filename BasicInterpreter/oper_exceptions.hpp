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