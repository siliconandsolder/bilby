/**	@file oper_exceptions.cpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of operation exception classes
*/

#include "oper_exceptions.hpp"
using namespace std;

/**
@name:		BinOpException
@purpose:	throws an exception for a binary operation
@param:		std::string, std::string, std::string
@return:	null
*/
BinOpException::BinOpException(std::string opert, std::string operOne, std::string operTwo)
{
	std::ostringstream os;
	os << std::string("Cannot perform <") << opert << std::string("> on a <") << operOne << std::string("> and a <") << operTwo << std::string(">");
	throw exception(os.str().c_str());
}

/**
@name:		EmptyVariableOper
@purpose:	throws an exception when an empty variable is found
@param:		std::string
@return:	null
*/
EmptyVariableOper::EmptyVariableOper(std::string oper)
{
	std::ostringstream os;
	os << std::string("Variable being used in a <") << oper << std::string("> operation is null");
	throw exception(os.str().c_str());
}

/**
@name:		UnOpException
@purpose:	throws an exception for a unary operation
@param:		std::string, std::string
@return:	null
*/
UnOpException::UnOpException(std::string opert, std::string oper)
{
	std::ostringstream os;
	os << std::string("Cannot perform <") << opert << std::string("> on a <") << oper << std::string(">");
	throw exception(os.str().c_str());
}
