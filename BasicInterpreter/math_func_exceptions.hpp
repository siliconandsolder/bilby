#pragma once
#include <sstream>
#include <exception>

class FuncException : public std::exception
{
private:
	std::string operation_, operand_;
public:
	FuncException(std::string operation, std::string operand);
};

class EmptyVariableException : public std::exception
{
private:
	std::string func_;
public:
	EmptyVariableException(std::string func);
};