/**	@file operator.cpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of Operator classes
*/

#include "operator.hpp"
#include "operation.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include "integer.hpp"
#include "float.hpp"
#include "boolean.hpp"
#include "word.hpp"
#include "oper_exceptions.hpp"
#include <stack>
#include <exception>


/**
@name:		perform
@purpose:	performs a factorial calculation on an operand
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Factorial::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto fac = valueStack.top();
	valueStack.pop();
	Operand::pointer_type fin;

	if (is<Integer>(fac))
	{
		auto val = getValue<Integer>(fac);
		Integer::value_type facVal;
		if (val > 0)
		{
			facVal = 1;

			while (val > 0)
			{
				facVal *= val;
				--val;
			}
		}
		else if (val < 0)
		{
			facVal = 1;

			while (val < 0)
			{
				facVal *= val;
				++val;
			}
		}
		else
		{
			facVal = val;
		}

		fin = make_operand<Integer>(facVal);
	}
	else if (is<Float>(fac))
	{
		throw UnOpException("Factorial", "Real");
	}
	else if (is<Boolean>(fac))
	{
		throw UnOpException("Factorial", "Boolean");
	}

	return fin;
}

/**
@name:		perform
@purpose:	makes a number positive
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Identity::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto oper = valueStack.top();
	valueStack.pop();

	if (is<Boolean>(oper))
		throw UnOpException("Identity", "Boolean");
	else if (is<Word>(oper))
		throw UnOpException("Identity", "Word");
	else if (is<Integer>(oper))
	{
		if (convert<Integer>(oper)->getValue() < 0)
			convert<Integer>(oper)->getValue() * -1;
	}
	else
	{
		if (convert<Float>(oper)->getValue() < 0)
			convert<Float>(oper)->getValue() * -1;
	}
	return oper;
}

/**
@name:		perform
@purpose:	makes a number negative
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Negation::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto oper = valueStack.top();
	Operand::pointer_type fin;
	valueStack.pop();

	if (is<Integer>(oper))
	{
		auto val = getValue<Integer>(oper);
		if (val > 0)
		{
			val *= -1;
		}
		fin = make_operand<Integer>(val);
	}
	else if (is<Float>(oper))
	{
		auto val = getValue<Float>(oper);
		if (val > 0)
		{
			val *= -1;
		}
		fin = make_operand<Float>(val);
	}
	else if (is<Boolean>(oper))
		throw UnOpException("Negation", "Boolean");
	else if (is<Word>(oper))
		throw UnOpException("Negation", "Word");

	return fin;
}

/**
@name:		perform
@purpose:	flips a boolean's state
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Not::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto notter = valueStack.top();
	Operand::pointer_type fin;
	valueStack.pop();

	if (is<Integer>(notter))
		throw UnOpException("Not", "Integer");
	else if (is<Float>(notter))
		throw UnOpException("Not", "Float");
	else if (is<Word>(notter))
		throw UnOpException("Not", "Word");
	else if (is<Boolean>(notter))
		fin = make_operand<Boolean>(!getValue<Boolean>(notter));

	return fin;
}

/** Raises one value to the power of the second value
@return Integer::value_type
@param Two Integer::value_type variables
*/
Integer::value_type int_power(Integer::value_type val, Integer::value_type _pow)
{
	if (_pow.is_zero())
		return 1;
	auto x = int_power(val, _pow / 2);
	if (_pow % 2 == 0)
		return x * x;
	else
		return val * x * x;
}

/**
@name:		perform
@purpose:	raises one number to the power of another
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Power::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto first = valueStack.top();
	valueStack.pop();
	auto second = valueStack.top();
	valueStack.pop();

	Operand::pointer_type fin;

	// possible first values
	Integer::value_type val_one_int;
	Float::value_type val_one_real;

	// possible second values
	Integer::value_type val_two_int;
	Float::value_type val_two_real;


	if (is<Integer>(first))
	{
		val_one_int = getValue<Integer>(first);

		if (is<Integer>(second))
		{
			val_two_int = getValue<Integer>(second);

			Integer::value_type finVal;
			Float::value_type finValR;

			if (val_two_int < 0)
			{
				finValR = boost::multiprecision::pow(static_cast<Float::value_type>(val_one_int), static_cast<Float::value_type>(val_two_int));
				fin = make_operand<Float>(finValR);
			}
			else
			{
				finVal = int_power(val_one_int, val_two_int);
				fin = make_operand<Integer>(finVal);
			}
		}
		else if (is<Float>(second))
		{
			val_two_real = getValue<Float>(second);
			Float::value_type finVal = boost::multiprecision::pow(static_cast<Float::value_type>(val_one_int), val_two_real);
			fin = make_operand<Float>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("Power", "Integer", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("Power", "Integer", "Word");
	}

	else if (is<Float>(first))
	{
		val_one_real = getValue<Float>(first);

		if (is<Integer>(second))
		{
			val_two_int = getValue<Integer>(second);
			Float::value_type finVal = boost::multiprecision::pow(val_one_real, static_cast<Float::value_type>(val_two_int));
			fin = make_operand<Float>(finVal);
		}
		else if (is<Float>(second))
		{
			val_two_real = getValue<Float>(second);
			Float::value_type finVal = boost::multiprecision::pow(val_one_real, val_two_real);
			fin = make_operand<Float>(finVal);
		}

		else if (is<Boolean>(second))
			throw BinOpException("Power", "Float", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("Power", "Float", "Word");

	}
	else if (is<Boolean>(first))
		throw BinOpException("Power", "Boolean", "any operator");
	else if (is<Word>(first))
		throw BinOpException("Power", "Word", "any operator");

	return fin;
}

/**
@name:		perform
@purpose:	adds two numbers
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Addition::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto first = valueStack.top();
	valueStack.pop();
	auto second = valueStack.top();
	valueStack.pop();
	Operand::pointer_type fin;

	// possible first values
	Integer::value_type val_one_int;
	Float::value_type val_one_real;
	Boolean::value_type val_one_bool;
	Word::value_type val_one_word;

	// possible second values
	Integer::value_type val_two_int;
	Float::value_type val_two_real;
	Boolean::value_type val_two_bool;
	Word::value_type val_two_word;


	if (is<Integer>(first))
	{
		val_one_int = getValue<Integer>(first);

		if (is<Integer>(second))
		{
			val_two_int = getValue<Integer>(second);
			Integer::value_type finVal = val_one_int + val_two_int;
			fin = make_operand<Integer>(finVal);
		}
		else if (is<Float>(second))
		{
			val_two_real = getValue<Float>(second);
			Float::value_type finVal = static_cast<Float::value_type>(val_one_int) + val_two_real;
			fin = make_operand<Float>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("Addition", "Integer", "Boolean");
		else if (is<Word>(second))
		{
			val_two_word = getValue<Word>(second);
			fin = make_operand<Word>(boost::lexical_cast<string_type>(val_one_int) + val_two_word);
		}
	}
	// if first value is a Real
	else if (is<Float>(first))
	{
		val_one_real = getValue<Float>(first);

		if (is<Integer>(second))
		{
			val_two_int = getValue<Integer>(second);
			Float::value_type finVal = val_one_real + static_cast<Float::value_type>(val_two_int);
			fin = make_operand<Float>(finVal);
		}
		else if (is<Float>(second))
		{
			val_two_real = getValue<Float>(second);
			Float::value_type finVal = val_one_real + val_two_real;
			fin = make_operand<Float>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("Addition", "Float", "Boolean");
		else if (is<Word>(second))
		{
			val_two_word = getValue<Word>(second);
			fin = make_operand<Word>(boost::lexical_cast<string_type>(val_one_real) + val_two_word);
		}

	}
	// if the first value is a Boolean
	else if (is<Boolean>(first))
	{
		val_one_bool = getValue<Boolean>(first);

		if (is<Integer>(second))
			throw BinOpException("Addition", "Boolean", "Integer");
		else if (is<Float>(second))
			throw BinOpException("Addition", "Boolean", "Float");
		else if (is<Word>(second))
		{
			string_type boolVal = val_one_bool == true ? "true" : "false";
			val_two_word = getValue<Word>(second);
			val_two_word = boolVal + val_two_word;
			fin = make_operand<Word>(val_two_word);
		}
		else if (is<Boolean>(second))
		{
			val_two_bool = getValue<Boolean>(second);
			bool finVal = val_one_bool || val_two_bool;
			fin = make_operand<Boolean>(finVal);
		}
	}
	else if (is<Word>(first))
	{
		val_one_word = getValue<Word>(first);
		
		if (is<Word>(second))
			fin = make_operand<Word>(val_one_word + getValue<Word>(second));
		else if (is<Integer>(second))
		{
			val_two_int = getValue<Integer>(second);
			fin = make_operand<Word>(val_one_word + boost::lexical_cast<string_type>(val_two_int));
		}
		else if (is<Float>(second))
		{
			val_two_real = getValue<Float>(second);
			fin = make_operand<Word>(val_one_word + boost::lexical_cast<string_type>(val_two_real));
		}
		else if (is<Boolean>(second))
		{
			val_two_bool = getValue<Boolean>(second);
			string_type boolVal = val_two_bool == true ? "true" : "false";
			fin = make_operand<Word>(val_one_word + boolVal);
		}
	}

	return fin;
}

/**
@name:		perform
@purpose:	subtracts two number
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Subtraction::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto first = valueStack.top();
	valueStack.pop();
	auto second = valueStack.top();
	valueStack.pop();
	Operand::pointer_type fin;

	// possible first values
	Integer::value_type val_one_int;
	Float::value_type val_one_real;

	// possible second values
	Integer::value_type val_two_int;
	Float::value_type val_two_real;


	if (is<Integer>(first))
	{
		val_one_int = getValue<Integer>(first);

		if (is<Integer>(second))
		{
			val_two_int = getValue<Integer>(second);
			Integer::value_type finVal = val_one_int - val_two_int;
			fin = make_operand<Integer>(finVal);
		}
		else if (is<Float>(second))
		{
			val_two_real = getValue<Float>(second);
			Float::value_type finVal = static_cast<Float::value_type>(val_one_int) - val_two_real;
			fin = make_operand<Float>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("Subtraction", "Integer", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("Subtraction", "Integer", "Word");
	}
	// if first value is a Real
	else if (is<Float>(first))
	{
		val_one_real = getValue<Float>(first);

		if (is<Integer>(second))
		{
			val_two_int = getValue<Integer>(second);
			Float::value_type finVal = val_one_real - static_cast<Float::value_type>(val_two_int);
			fin = make_operand<Float>(finVal);
		}
		else if (is<Float>(second))
		{
			val_two_real = getValue<Float>(second);
			Float::value_type finVal = val_one_real - val_two_real;
			fin = make_operand<Float>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("Subtraction", "Float", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("Subtraction", "Float", "Word");
	}
	else if (is<Boolean>(first))
		throw BinOpException("Subtraction", "Boolean", "Any Operand");
	else if (is<Word>(first))
		throw BinOpException("Subtraction", "Word", "Any Operand");

	return fin;
}

/**
@name:		perform
@purpose:	multiplies two number
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Multiplication::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto first = valueStack.top();
	valueStack.pop();
	auto second = valueStack.top();
	valueStack.pop();
	Operand::pointer_type fin;

	// possible first values
	Integer::value_type val_one_int;
	Float::value_type val_one_real;

	// possible second values
	Integer::value_type val_two_int;
	Float::value_type val_two_real;


	if (is<Integer>(first))
	{
		val_one_int = getValue<Integer>(first);

		if (is<Integer>(second))
		{
			val_two_int = getValue<Integer>(second);
			Integer::value_type finVal = val_one_int * val_two_int;
			fin = make_operand<Integer>(finVal);
		}
		else if (is<Float>(second))
		{
			val_two_real = getValue<Float>(second);
			Float::value_type finVal = static_cast<Float::value_type>(val_one_int) * val_two_real;
			fin = make_operand<Float>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("Multiplication", "Integer", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("Multiplication", "Integer", "Word");
	}
	// if first value is a Real
	else if (is<Float>(first))
	{
		val_one_real = getValue<Float>(first);

		if (is<Integer>(second))
		{
			val_two_int = getValue<Integer>(second);
			Float::value_type finVal = val_one_real * static_cast<Float::value_type>(val_two_int);
			fin = make_operand<Float>(finVal);
		}
		else if (is<Float>(second))
		{
			val_two_real = getValue<Float>(second);
			Float::value_type finVal = val_one_real * val_two_real;
			fin = make_operand<Float>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("Multiplication", "Float", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("Multiplication", "Float", "Word");
	}
	// if the first value is a Boolean
	else if (is<Boolean>(first))
		throw BinOpException("Multiplication", "Boolean", "Any Operand");
	else if (is<Word>(first))
		throw BinOpException("Multiplication", "Word", "Any Operand");

	return fin;
}

/**
@name:		perform
@purpose:	divides two numbers
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Division::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto first = valueStack.top();
	valueStack.pop();
	auto second = valueStack.top();
	valueStack.pop();
	Operand::pointer_type fin;

	// possible first values
	Integer::value_type val_one_int;
	Float::value_type val_one_real;

	// possible second values
	Integer::value_type val_two_int;
	Float::value_type val_two_real;


	if (is<Integer>(first))
	{
		val_one_int = getValue<Integer>(first);

		if (is<Integer>(second))
		{
			val_two_int = getValue<Integer>(second);
			Integer::value_type finVal = val_one_int / val_two_int;
			fin = make_operand<Integer>(finVal);
		}
		else if (is<Float>(second))
		{
			val_two_real = getValue<Float>(second);
			Float::value_type finVal = static_cast<Float::value_type>(val_one_int) / val_two_real;
			fin = make_operand<Float>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("Division", "Integer", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("Division", "Integer", "Word");
	}
	// if first value is a Real
	else if (is<Float>(first))
	{
		val_one_real = getValue<Float>(first);

		if (is<Integer>(second))
		{
			val_two_int = getValue<Integer>(second);
			Float::value_type finVal = val_one_real / static_cast<Float::value_type>(val_two_int);
			fin = make_operand<Float>(finVal);
		}
		else if (is<Float>(second))
		{
			val_two_real = getValue<Float>(second);
			Float::value_type finVal = val_one_real / val_two_real;
			fin = make_operand<Float>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("Division", "Float", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("Division", "Float", "Word");
	}
	// if the first value is a Boolean
	else if (is<Boolean>(first))
		throw BinOpException("Division", "Boolean", "Any Operand");
	else if (is<Word>(first))
		throw BinOpException("Division", "Word", "Any Operand");

	return fin;
}

/**
@name:		perform
@purpose:	performs a modulo operation on two number
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Modulus::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto first = valueStack.top();
	valueStack.pop();
	auto second = valueStack.top();
	valueStack.pop();

	Operand::pointer_type fin;

	// possible first values
	Integer::value_type val_one_int;

	// possible second values
	Integer::value_type val_two_int;


	if (is<Integer>(first))
	{
		val_one_int = getValue<Integer>(first);

		if (is<Integer>(second))
		{
			val_two_int = getValue<Integer>(second);
			Integer::value_type finVal = val_one_int % val_two_int;
			fin = make_operand<Integer>(finVal);
		}
		else if (is<Float>(second))
			throw BinOpException("Modulus", "Integer", "Real");
		else if (is<Boolean>(second))
			throw BinOpException("Modulus", "Integer", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("Modulus", "Integer", "Word");
	}
	// if first value is a Real
	else if (is<Float>(first))
		throw BinOpException("Modulus", "Float", "Any Operand");
	// if the first value is a Boolean
	else if (is<Boolean>(first))
		throw BinOpException("Modulus", "Boolean", "Any Operand");
	else if (is<Word>(first))
		throw BinOpException("Modulus", "Word", "Any Operand");

	return fin;
}

/**
@name:		perform
@purpose:	performs and And operation on two booleans
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type And::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto first = valueStack.top();
	valueStack.pop();
	auto second = valueStack.top();
	valueStack.pop();

	Operand::pointer_type fin;

	if (is<Integer>(first))
	{
		throw UnOpException("And", "Integer");
	}
	else if (is<Float>(first))
	{
		throw UnOpException("And", "Float");
	}
	else if (is<Boolean>(first))
	{
		auto val_one = getValue<Boolean>(first);

		if (is<Integer>(second))
			throw BinOpException("And", "Boolean", "Integer");
		else if (is<Float>(second))
			throw BinOpException("And", "Boolean", "Float");
		else if (is<Word>(second))
			throw BinOpException("And", "Boolean", "Word");
		else if (is<Boolean>(second))
		{
			auto val_two = getValue<Boolean>(second);
			auto finVal = (val_one && val_two);

			fin = make_operand<Boolean>(finVal);
		}
	}
	else if (is<Word>(first))
		throw BinOpException("And", "Word", "Any Operand");

	return fin;
}

/**
@name:		perform
@purpose:	performs and Or operation on two booleans
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Or::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto first = valueStack.top();
	valueStack.pop();
	auto second = valueStack.top();
	valueStack.pop();

	Operand::pointer_type fin;

	if (is<Integer>(first))
		throw UnOpException("Or", "Integer");
	else if (is<Float>(first))
		throw UnOpException("Or", "Float");
	else if (is<Boolean>(first))
	{
		auto val_one = getValue<Boolean>(first);

		if (is<Integer>(second))
			throw BinOpException("Or", "Boolean", "Integer");
		else if (is<Float>(second))
			throw BinOpException("Or", "Boolean", "Float");
		else if (is<Word>(second))
			throw BinOpException("Or", "Boolean", "Word");
		else if (is<Boolean>(second))
		{
			auto val_two = getValue<Boolean>(second);
			auto finVal = (val_one || val_two);

			fin = make_operand<Boolean>(finVal);
		}
	}
	else if (is<Word>(first))
		throw BinOpException("Or", "Word", "Any Operand");

	return fin;
}

/**
@name:		perform
@purpose:	performs and Nor operation on two booleans
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Nor::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto first = valueStack.top();
	valueStack.pop();
	auto second = valueStack.top();
	valueStack.pop();

	Operand::pointer_type fin;

	if (is<Integer>(first))
		throw UnOpException("Nor", "Integer");
	else if (is<Float>(first))
		throw UnOpException("Nor", "Float");
	else if (is<Boolean>(first))
	{
		auto val_one = getValue<Boolean>(first);

		if (is<Integer>(second))
			throw BinOpException("Nor", "Boolean", "Integer");
		else if (is<Float>(second))
			throw BinOpException("Nor", "Boolean", "Float");
		else if (is<Word>(second))
			throw BinOpException("Nor", "Boolean", "Word");
		else if (is<Boolean>(second))
		{
			auto val_two = getValue<Boolean>(second);
			auto finVal = !(val_one || val_two);

			fin = make_operand<Boolean>(finVal);
		}
	}
	else if (is<Word>(first))
		throw BinOpException("Nor", "Word", "Any Operand");

	return fin;
}

/**
@name:		perform
@purpose:	performs and Xor operation on two booleans
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Xor::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto first = valueStack.top();
	valueStack.pop();
	auto second = valueStack.top();
	valueStack.pop();

	Operand::pointer_type fin;

	if (is<Integer>(first))
	{
		throw UnOpException("Xor", "Integer");
	}
	else if (is<Float>(first))
	{
		throw UnOpException("Xor", "Float");
	}
	else if (is<Boolean>(first))
	{
		auto val_one = getValue<Boolean>(first);

		if (is<Integer>(second))
			throw BinOpException("Xor", "Boolean", "Integer");
		else if (is<Float>(second))
			throw BinOpException("Xor", "Boolean", "Float");
		else if (is<Word>(second))
			throw BinOpException("Xor", "Boolean", "Word");
		else if (is<Boolean>(second))
		{
			auto val_two = getValue<Boolean>(second);
			auto finVal = ((val_one || val_two) && !(val_one && val_two));

			fin = make_operand<Boolean>(finVal);
		}
	}
	else if (is<Word>(first))
		throw BinOpException("Xor", "Word", "Any Operand");

	return fin;
}

/**
@name:		perform
@purpose:	performs and Xnor operation on two booleans
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Xnor::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto first = valueStack.top();
	valueStack.pop();
	auto second = valueStack.top();
	valueStack.pop();

	Operand::pointer_type fin;

	if (is<Integer>(first))
	{
		throw UnOpException("Xnor", "Integer");
	}
	else if (is<Float>(first))
	{
		throw UnOpException("Xnor", "Float");
	}
	else if (is<Boolean>(first))
	{
		auto val_one = getValue<Boolean>(first);

		if (is<Integer>(second))
			throw BinOpException("Xnor", "Boolean", "Integer");
		else if (is<Float>(second))
			throw BinOpException("Xnor", "Boolean", "Float");
		else if (is<Word>(second))
			throw BinOpException("Xnor", "Boolean", "Word");
		else if (is<Boolean>(second))
		{
			auto val_two = getValue<Boolean>(second);
			auto finVal = !((val_one || val_two) && !(val_one && val_two));

			fin = make_operand<Boolean>(finVal);
		}
	}
	else if (is<Word>(first))
		throw BinOpException("Xnor", "Word", "Any Operand");

	return fin;
}

/**
@name:		perform
@purpose:	compares two operands that are the same type
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Equality::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto first = valueStack.top();
	valueStack.pop();
	auto second = valueStack.top();
	valueStack.pop();

	Operand::pointer_type fin;

	if (is<Integer>(first))
	{
		auto val_one = getValue<Integer>(first);

		if (is<Integer>(second))
		{
			auto val_two = getValue<Integer>(second);
			bool finVal = (val_one == val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Float>(second))
		{
			auto val_two = getValue<Float>(second);
			bool finVal = (static_cast<Float::value_type>(val_one) == val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("Equality", "Integer", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("Equality", "Integer", "Word");
	}
	else if (is<Float>(first))
	{
		auto val_one = getValue<Float>(first);

		if (is<Integer>(second))
		{
			auto val_two = getValue<Integer>(second);
			bool finVal = (val_one == static_cast<Float::value_type>(val_two));
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Float>(second))
		{
			auto val_two = getValue<Float>(second);
			bool finVal = (val_one == val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("Equality", "Float", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("Equality", "Float", "Word");

	}
	else if (is<Boolean>(first))
	{
		auto val_one = getValue<Boolean>(first);

		if (is<Integer>(second))
			throw BinOpException("Equality", "Boolean", "Integer");
		else if (is<Float>(second))
			throw BinOpException("Equality", "Boolean", "Float");
		else if (is<Boolean>(second))
		{
			auto val_two = getValue<Boolean>(second);
			auto finVal = (val_one == val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Word>(second))
			throw BinOpException("Equality", "Boolean", "Word");
	}
	else if (is<Word>(first))
		throw BinOpException("Equality", "Word", "Any Operand");

	return fin;
}

/**
@name:		perform
@purpose:	checks if one number is greater than another
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Greater::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto first = valueStack.top();
	valueStack.pop();
	auto second = valueStack.top();
	valueStack.pop();

	Operand::pointer_type fin;

	if (is<Integer>(first))
	{
		auto val_one = getValue<Integer>(first);

		if (is<Integer>(second))
		{
			auto val_two = getValue<Integer>(second);
			bool finVal = (val_one > val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Float>(second))
		{
			auto val_two = getValue<Float>(second);
			bool finVal = (static_cast<Float::value_type>(val_one) > val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("Greater", "Integer", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("Greater", "Integer", "Word");
	}
	else if (is<Float>(first))
	{
		auto val_one = getValue<Float>(first);

		if (is<Integer>(second))
		{
			auto val_two = getValue<Integer>(second);
			bool finVal = (val_one > static_cast<Float::value_type>(val_two));
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Float>(second))
		{
			auto val_two = getValue<Float>(second);
			bool finVal = (val_one > val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("Greater", "Float", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("Greater", "Float", "Word");

	}
	else if (is<Boolean>(first))
	{
		auto val_one = getValue<Boolean>(first);

		if (is<Integer>(second))
			throw BinOpException("Greater", "Boolean", "Integer");
		else if (is<Float>(second))
			throw BinOpException("Greater", "Boolean", "Float");
		else if (is<Boolean>(second))
			throw BinOpException("Greater", "Boolean", "Any Operand");
		else if (is<Word>(second))
			throw BinOpException("Greater", "Boolean", "Word");
	}
	else if (is<Word>(first))
		throw BinOpException("Greater", "Word", "Any Operand");

	return fin;
}

/**
@name:		perform
@purpose:	checks if one number is greater than or equal to another
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type GreaterEqual::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto first = valueStack.top();
	valueStack.pop();
	auto second = valueStack.top();
	valueStack.pop();

	Operand::pointer_type fin;

	if (is<Integer>(first))
	{
		auto val_one = getValue<Integer>(first);

		if (is<Integer>(second))
		{
			auto val_two = getValue<Integer>(second);
			bool finVal = (val_one >= val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Float>(second))
		{
			auto val_two = getValue<Float>(second);
			bool finVal = (static_cast<Float::value_type>(val_one) >= val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("GreaterEqual", "Integer", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("GreaterEqual", "Integer", "Word");
	}
	else if (is<Float>(first))
	{
		auto val_one = getValue<Float>(first);

		if (is<Integer>(second))
		{
			auto val_two = getValue<Integer>(second);
			bool finVal = (val_one >= static_cast<Float::value_type>(val_two));
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Float>(second))
		{
			auto val_two = getValue<Float>(second);
			bool finVal = (val_one >= val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("GreaterEqual", "Float", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("GreaterEqual", "Float", "Word");

	}
	else if (is<Boolean>(first))
	{
		auto val_one = getValue<Boolean>(first);

		if (is<Integer>(second))
			throw BinOpException("GreaterEqual", "Boolean", "Integer");
		else if (is<Float>(second))
			throw BinOpException("GreaterEqual", "Boolean", "Float");
		else if (is<Boolean>(second))
			throw BinOpException("GreaterEqual", "Boolean", "Any Operand");
		else if (is<Word>(second))
			throw BinOpException("GreaterEqual", "Boolean", "Word");
	}
	else if (is<Word>(first))
		throw BinOpException("GreaterEqual", "Word", "Any Operand");

	return fin;
}

/**
@name:		perform
@purpose:	checks if two operands are not equal
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Inequality::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto first = valueStack.top();
	valueStack.pop();
	auto second = valueStack.top();
	valueStack.pop();

	Operand::pointer_type fin;

	if (is<Integer>(first))
	{
		auto val_one = getValue<Integer>(first);

		if (is<Integer>(second))
		{
			auto val_two = getValue<Integer>(second);
			bool finVal = (val_one != val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Float>(second))
		{
			auto val_two = getValue<Float>(second);
			bool finVal = (static_cast<Float::value_type>(val_one) != val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("Inequality", "Integer", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("Inequality", "Integer", "Word");
	}
	else if (is<Float>(first))
	{
		auto val_one = getValue<Float>(first);

		if (is<Integer>(second))
		{
			auto val_two = getValue<Integer>(second);
			bool finVal = (val_one != static_cast<Float::value_type>(val_two));
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Float>(second))
		{
			auto val_two = getValue<Float>(second);
			bool finVal = (val_one != val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("Inequality", "Float", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("Inequality", "Float", "Word");

	}
	else if (is<Boolean>(first))
	{
		auto val_one = getValue<Boolean>(first);

		if (is<Integer>(second))
			throw BinOpException("Inequality", "Boolean", "Integer");
		else if (is<Float>(second))
			throw BinOpException("Inequality", "Boolean", "Float");
		else if (is<Boolean>(second))
			throw BinOpException("Inequality", "Boolean", "Any Operand");
		else if (is<Word>(second))
			throw BinOpException("Inequality", "Float", "Word");
	}
	else if (is<Word>(first))
		throw BinOpException("Inequality", "Word", "Any Operand");

	return fin;
}

/**
@name:		perform
@purpose:	checks if one number is less than another
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Less::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto first = valueStack.top();
	valueStack.pop();
	auto second = valueStack.top();
	valueStack.pop();

	Operand::pointer_type fin;

	if (is<Integer>(first))
	{
		auto val_one = getValue<Integer>(first);

		if (is<Integer>(second))
		{
			auto val_two = getValue<Integer>(second);
			bool finVal = (val_one < val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Float>(second))
		{
			auto val_two = getValue<Float>(second);
			bool finVal = (static_cast<Float::value_type>(val_one) < val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("Less", "Integer", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("Less", "Integer", "Word");
	}
	else if (is<Float>(first))
	{
		auto val_one = getValue<Float>(first);

		if (is<Integer>(second))
		{
			auto val_two = getValue<Integer>(second);
			bool finVal = (val_one < static_cast<Float::value_type>(val_two));
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Float>(second))
		{
			auto val_two = getValue<Float>(second);
			bool finVal = (val_one < val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("Less", "Float", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("Less", "Float", "Word");

	}
	else if (is<Boolean>(first))
	{
		auto val_one = getValue<Boolean>(first);

		if (is<Integer>(second))
			throw BinOpException("Less", "Boolean", "Integer");
		else if (is<Float>(second))
			throw BinOpException("Less", "Boolean", "Float");
		else if (is<Boolean>(second))
			throw BinOpException("Less", "Boolean", "Any Operand");
		else if (is<Word>(second))
			throw BinOpException("Less", "Boolean", "Word");
	}
	else if (is<Word>(first))
		throw BinOpException("Less", "Word", "Any Operand");

	return fin;
}

/**
@name:		perform
@purpose:	checks if one number is less than or equal to another
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type LessEqual::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto first = valueStack.top();
	valueStack.pop();
	auto second = valueStack.top();
	valueStack.pop();

	Operand::pointer_type fin;

	if (is<Integer>(first))
	{
		auto val_one = getValue<Integer>(first);

		if (is<Integer>(second))
		{
			auto val_two = getValue<Integer>(second);
			bool finVal = (val_one <= val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Float>(second))
		{
			auto val_two = getValue<Float>(second);
			bool finVal = (static_cast<Float::value_type>(val_one) <= val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("LessEqual", "Integer", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("LessEqual", "Integer", "Word");
	}
	else if (is<Float>(first))
	{
		auto val_one = getValue<Float>(first);

		if (is<Integer>(second))
		{
			auto val_two = getValue<Integer>(second);
			bool finVal = (val_one <= static_cast<Float::value_type>(val_two));
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Float>(second))
		{
			auto val_two = getValue<Float>(second);
			bool finVal = (val_one <= val_two);
			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Boolean>(second))
			throw BinOpException("LessEqual", "Float", "Boolean");
		else if (is<Word>(second))
			throw BinOpException("LessEqual", "Float", "Word");

	}
	else if (is<Boolean>(first))
	{
		auto val_one = getValue<Boolean>(first);

		if (is<Integer>(second))
			throw BinOpException("LessEqual", "Boolean", "Integer");
		else if (is<Float>(second))
			throw BinOpException("LessEqual", "Boolean", "Float");
		else if (is<Boolean>(second))
			throw BinOpException("LessEqual", "Boolean", "Any Operand");
		else if (is<Word>(second))
			throw BinOpException("LessEqual", "Boolean", "Word");
	}
	else if (is<Word>(first))
		throw BinOpException("LessEqual", "Word", "Any Operand");

	return fin;
}

/**
@name:		perform
@purpose:	perfoms a Nand operation on two booleans
@param:		std::stack<Operand::pointer_type>&
@return:	Operand::pointer_type
*/
Operand::pointer_type Nand::perform(std::stack<Operand::pointer_type>& valueStack)
{
	auto first = valueStack.top();
	valueStack.pop();
	auto second = valueStack.top();
	valueStack.pop();

	Operand::pointer_type fin;

	if (is<Integer>(first))
		throw UnOpException("Nand", "Integer");
	else if (is<Float>(first))
		throw UnOpException("Nand", "Float");
	else if (is<Boolean>(first))
	{
		auto val_one = getValue<Boolean>(first);

		if (is<Integer>(second))
			throw BinOpException("Nand", "Boolean", "Integer");
		else if (is<Float>(second))
			throw BinOpException("Nand", "Boolean", "Float");
		else if (is<Boolean>(second))
		{
			auto val_two = getValue<Boolean>(second);
			auto finVal = !(val_one && val_two);

			fin = make_operand<Boolean>(finVal);
		}
		else if (is<Word>(second))
			throw BinOpException("Nand", "Boolean", "Word");
	}
	else if (is<Word>(first))
		throw BinOpException("Nand", "Word", "Any Operand");

	return fin;
}
