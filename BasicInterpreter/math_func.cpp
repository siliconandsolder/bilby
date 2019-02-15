/**	@file math_func.cpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of MathFunc
*/

#include "math_func.hpp"
#include "math_func_exceptions.hpp"
#include "integer.hpp"
#include "float.hpp"
#include "boolean.hpp"
#include "word.hpp"

/**
@name:		call
@purpose:	Returns the first argument to the power of the second
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Pow::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	return Token::pointer_type();
}

/**
@name:		call
@purpose:	Returns the tangent of a single argument
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Tan::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	return Token::pointer_type();
}

/**
@name:		call
@purpose:	Returns the square root of a single argument
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Sqr::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	return Token::pointer_type();
}

/**
@name:		call
@purpose:	Returns the sine of a single argument
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Sin::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	return Token::pointer_type();
}

/**
@name:		call
@purpose:	Returns a random number between two arguments
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Rnd::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	return Token::pointer_type();
}

/**
@name:		call
@purpose:	Returns the log of a single argument
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Log::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	return Token::pointer_type();
}

/**
@name:		call
@purpose:	Performs an exponential function on one argument
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Expo::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	return Token::pointer_type();
}

/**
@name:		call
@purpose:	Returns the cosine of a single argument
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Cos::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	return Token::pointer_type();
}


/**
@name:		call
@purpose:	Returns the arctangent of a single argument
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Arctan::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	return Token::pointer_type();
}

/**
@name:		call
@purpose:	Returns the absolute value of a single argument
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Abs::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	auto num = args.front();

	if (is<Integer>(num))
	{
		auto numVal = getValue<Integer>(num);
		return tok_p(int_p(new Integer(boost::multiprecision::abs(numVal))));
	}
	else if (is<Float>(num))
	{
		auto numVal = getValue<Float>(num);
		return tok_p(float_p(new Float(boost::multiprecision::abs(numVal))));
	}
	else if (is<Boolean>(num))
	{
		throw FuncException("Abs()", "Boolean");
	}
	else if (is<Word>(num))
	{
		throw FuncException("Abs()", "Word");
	}
}
