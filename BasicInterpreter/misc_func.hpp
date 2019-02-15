/**	@file misc_func.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif some miscellaneous native functions available in global scope
*/

#pragma once
#include "beta_callable.hpp"

class DateTime : public BetaCallable
{
public:
	DEF_POINTER_TYPE(DateTime)
	DEF_IS_CONVERTIBLE_FROM(DateTime)

	virtual unsigned short arity() { return 0; }
	virtual Token::pointer_type call(Interpreter * interpreter, std::list<Token::pointer_type> args);
	virtual string_type toString() { return "<func | native | DateTime>"; }
};

class DateTimeUTC : public BetaCallable
{
public:
	DEF_POINTER_TYPE(DateTimeUTC)
	DEF_IS_CONVERTIBLE_FROM(DateTimeUTC)

	virtual unsigned short arity() { return 0; }
	virtual Token::pointer_type call(Interpreter * interpreter, std::list<Token::pointer_type> args);
	virtual string_type toString() { return "<func | native | DateTimeUTC>"; }
};

class Epoch : public BetaCallable
{
public:
	DEF_POINTER_TYPE(Epoch)
	DEF_IS_CONVERTIBLE_FROM(Epoch)

	virtual unsigned short arity() { return 0; }
	virtual Token::pointer_type call(Interpreter * interpreter, std::list<Token::pointer_type> args);
	virtual string_type toString() { return "<func | native | Epoch>"; }
};