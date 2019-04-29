/**	@file math_func.cpp
	@author Garth Santor, modified by Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of MathFunc
*/

#include <boost/random.hpp>
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
	auto num = args.front();
	auto pow = args.back();

	if (is<Integer>(num) || is<Float>(num))
	{
		Float::value_type numVal;
		Float::value_type powVal;
		
		if (is<Integer>(num))
			numVal = static_cast<Float::value_type>(getValue<Integer>(num));
		else
			numVal = static_cast<Float::value_type>(getValue<Float>(num));

		if (is<Integer>(pow) || is<Float>(pow))
		{
			if (is<Integer>(pow))
				powVal = static_cast<Float::value_type>(getValue<Integer>(pow));
			else
				powVal = static_cast<Float::value_type>(getValue<Float>(pow));
			
			if (is<Integer>(num) && is<Integer>(pow))
				return tok_p(int_p(new Integer(static_cast<Integer::value_type>(boost::multiprecision::pow(numVal, powVal)))));
			else
				return tok_p(float_p(new Float(boost::multiprecision::pow(numVal, powVal))));
		}
		else if (is<Boolean>(pow))
		{
			throw FuncException("Pow()", "Boolean");
		}
		else
		{
			throw FuncException("Pow()", "Word");
		}
	}
	else if (is<Boolean>(num))
	{
		throw FuncException("Pow()", "Boolean");
	}
	else
	{
		throw FuncException("Pow()", "Word");
	}
}

/**
@name:		call
@purpose:	Returns the tangent of a single argument
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Tan::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	auto num = args.front();

	if (is<Integer>(num))
	{
		auto numVal = static_cast<Float::value_type>(getValue<Integer>(num));
		return tok_p(int_p(new Integer(static_cast<Integer::value_type>(boost::multiprecision::tan(numVal)))));
	}
	else if (is<Float>(num))
	{
		auto numVal = getValue<Float>(num);
		return tok_p(float_p(new Float(boost::multiprecision::tan(numVal))));
	}
	else if (is<Boolean>(num))
	{
		throw FuncException("Tan()", "Boolean");
	}
	else
	{
		throw FuncException("Tan()", "Word");
	}
}

/**
@name:		call
@purpose:	Returns the square root of a single argument
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Sqr::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	auto num = args.front();

	if (is<Integer>(num))
	{
		auto numVal = static_cast<Float::value_type>(getValue<Integer>(num));
		return tok_p(int_p(new Integer(static_cast<Integer::value_type>(boost::multiprecision::sqrt(numVal)))));
	}
	else if (is<Float>(num))
	{
		auto numVal = getValue<Float>(num);
		return tok_p(float_p(new Float(boost::multiprecision::sqrt(numVal))));
	}
	else if (is<Boolean>(num))
	{
		throw FuncException("Sqr()", "Boolean");
	}
	else
	{
		throw FuncException("Sqr()", "Word");
	}
}

/**
@name:		call
@purpose:	Returns the sine of a single argument
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Sin::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	auto num = args.front();

	if (is<Integer>(num))
	{
		auto numVal = static_cast<Float::value_type>(getValue<Integer>(num));
		return tok_p(int_p(new Integer(static_cast<Integer::value_type>(boost::multiprecision::sin(numVal)))));
	}
	else if (is<Float>(num))
	{
		auto numVal = getValue<Float>(num);
		return tok_p(float_p(new Float(boost::multiprecision::sin(numVal))));
	}
	else if (is<Boolean>(num))
	{
		throw FuncException("Sin()", "Boolean");
	}
	else
	{
		throw FuncException("Sin()", "Word");
	}
}

/**
@name:		call
@purpose:	Returns a random number between two arguments
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Rnd::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	auto beg = args.front();
	auto end = args.back();
	boost::random::mt19937 rnd;

	if (is<Integer>(beg))
	{
		auto begVal = getValue<Integer>(beg);

		if (is<Integer>(end))
		{
			auto endVal = getValue<Integer>(end);
			boost::random::uniform_int_distribution<Integer::value_type> gen(begVal, endVal);
			return tok_p(int_p(new Integer(static_cast<Integer::value_type>((gen(rnd))))));
		}
		else if (is<Float>(end))
		{
			auto endVal = getValue<Float>(end);
			boost::random::uniform_real_distribution<Float::value_type> gen(static_cast<Float::value_type>(begVal), endVal);
			return tok_p(float_p(new Float(Float::value_type(gen(rnd)))));
		}
		else if (is<Boolean>(end))
		{
			throw FuncException("Rnd()", "Boolean");
		}
		else
		{
			throw FuncException("Rnd()", "Word");
		}
	}
	else if (is<Float>(beg))
	{
		auto begVal = getValue<Float>(beg);
		Float::value_type endVal;

		if (is<Integer>(end))
			endVal = static_cast<Float::value_type>(getValue<Integer>(end));
		else
			endVal = static_cast<Float::value_type>(getValue<Float>(end));

		if (is<Integer>(end) || is<Float>(end))
		{
			boost::random::uniform_real_distribution<Float::value_type> gen(begVal, endVal);
			return tok_p(int_p(new Integer(static_cast<Integer::value_type>(gen(rnd)))));
		}
		else if (is<Boolean>(end))
		{
			throw FuncException("Rnd()", "Boolean");
		}
		else
		{
			throw FuncException("Rnd()", "Word");
		}
	}
	else if (is<Boolean>(beg))
	{
		throw FuncException("Rnd()", "Boolean");
	}
	else
	{
		throw FuncException("Rnd()", "Word");
	}
}

/**
@name:		call
@purpose:	Returns the log of a single argument
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Log::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	auto num = args.front();

	if (is<Integer>(num))
	{
		auto numVal = getValue<Integer>(num);
		return tok_p(int_p(new Integer(static_cast<Integer::value_type>(boost::multiprecision::log10(static_cast<Float::value_type>(numVal))))));
	}
	else if (is<Float>(num))
	{
		auto numVal = getValue<Float>(num);
		return tok_p(float_p(new Float(Float::value_type(boost::multiprecision::log10(numVal)))));
	}
	else if (is<Boolean>(num))
	{
		throw FuncException("Log()", "Boolean");
	}
	else
	{
		throw FuncException("Log()", "Word");
	}
}

/**
@name:		call
@purpose:	Performs an exponential function on one argument
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Expo::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	auto num = args.front();

	if (is<Integer>(num))
	{
		auto numVal = getValue<Integer>(num);
		return tok_p(int_p(new Integer(static_cast<Integer::value_type>(boost::multiprecision::exp(static_cast<Float::value_type>(numVal))))));
	}
	else if (is<Float>(num))
	{
		auto numVal = getValue<Float>(num);
		return tok_p(float_p(new Float(Float::value_type(boost::multiprecision::exp(numVal)))));
	}
	else if (is<Boolean>(num))
	{
		throw FuncException("Expo()", "Boolean");
	}
	else
	{
		throw FuncException("Expo()", "Word");
	}
}

/**
@name:		call
@purpose:	Returns the cosine of a single argument
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Cos::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	auto num = args.front();

	if (is<Integer>(num))
	{
		auto numVal = getValue<Integer>(num);
		return tok_p(int_p(new Integer(static_cast<Integer::value_type>(boost::multiprecision::cos(static_cast<Float::value_type>(numVal))))));
	}
	else if (is<Float>(num))
	{
		auto numVal = getValue<Float>(num);
		return tok_p(float_p(new Float(Float::value_type(boost::multiprecision::cos(numVal)))));
	}
	else if (is<Boolean>(num))
	{
		throw FuncException("Cos()", "Boolean");
	}
	else
	{
		throw FuncException("Cos()", "Word");
	}
}


/**
@name:		call
@purpose:	Returns the arctangent of a single argument
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Arctan::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	auto num = args.front();

	if (is<Integer>(num))
	{
		auto numVal = getValue<Integer>(num);
		return tok_p(int_p(new Integer(static_cast<Integer::value_type>(boost::multiprecision::atan(static_cast<Float::value_type>(numVal))))));
	}
	else if (is<Float>(num))
	{
		auto numVal = getValue<Float>(num);
		return tok_p(float_p(new Float(Float::value_type(boost::multiprecision::atan(numVal)))));
	}
	else if (is<Boolean>(num))
	{
		throw FuncException("Arctan()", "Boolean");
	}
	else
	{
		throw FuncException("Arctan()", "Word");
	}
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
		return tok_p(int_p(new Integer(static_cast<Integer::value_type>(boost::multiprecision::abs(numVal)))));
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
	else
	{
		throw FuncException("Abs()", "Word");
	}
}
