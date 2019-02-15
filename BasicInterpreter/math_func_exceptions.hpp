/**	@file math_func_exceptions.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif custom exceptions for math functions
*/

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