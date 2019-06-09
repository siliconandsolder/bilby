/**	@file math_func.hpp
	@author Garth Santor, modified by Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif a series of native math functions that will be in global scope
*/

#pragma once

#include "beta_callable.hpp"

class Abs : public BetaCallable
{
public:
	DEF_POINTER_TYPE(Abs)
	DEF_IS_CONVERTIBLE_FROM(Abs)

	virtual unsigned short arity() override { return 1; }
	virtual Token::pointer_type call(Interpreter * interpreter, std::list<Token::pointer_type> args) override;
	virtual string_type toString() { return "<func | native | Absolute>"; }
};

class Arctan : public BetaCallable
{
public:
	DEF_POINTER_TYPE(Arctan)
	DEF_IS_CONVERTIBLE_FROM(Arctan)

	virtual unsigned short arity() override { return 1; }
	virtual Token::pointer_type call(Interpreter * interpreter, std::list<Token::pointer_type> args) override;
	virtual string_type toString() { return "<func | native | Arctangent>"; }
};

class Cos : public BetaCallable
{
public:
	DEF_POINTER_TYPE(Cos)
	DEF_IS_CONVERTIBLE_FROM(Cos)

	virtual unsigned short arity() override { return 1; }
	virtual Token::pointer_type call(Interpreter * interpreter, std::list<Token::pointer_type> args) override;
	virtual string_type toString() { return "<func | native | Cosine>"; }
};

class Expo : public BetaCallable
{
public:
	DEF_POINTER_TYPE(Expo)
	DEF_IS_CONVERTIBLE_FROM(Expo)

	virtual unsigned short arity() override { return 1; }
	virtual Token::pointer_type call(Interpreter * interpreter, std::list<Token::pointer_type> args) override;
	virtual string_type toString() { return "<func | native | Exponent>"; }
};

class Log : public BetaCallable
{
public:
	DEF_POINTER_TYPE(Log)
	DEF_IS_CONVERTIBLE_FROM(Log)

	virtual unsigned short arity() override { return 1; }
	virtual Token::pointer_type call(Interpreter * interpreter, std::list<Token::pointer_type> args) override;
	virtual string_type toString() { return "<func | native | Natural Logarithm>"; }
};

class Rnd : public BetaCallable
{
public:
	DEF_POINTER_TYPE(Rnd)
	DEF_IS_CONVERTIBLE_FROM(Rnd)

	virtual unsigned short arity() override { return 2; }
	virtual Token::pointer_type call(Interpreter * interpreter, std::list<Token::pointer_type> args) override;
	virtual string_type toString() { return "<func | native | Random>"; }
};

class Sin : public BetaCallable
{
public:
	DEF_POINTER_TYPE(Sin)
	DEF_IS_CONVERTIBLE_FROM(Sin)

	virtual unsigned short arity() override { return 1; }
	virtual Token::pointer_type call(Interpreter * interpreter, std::list<Token::pointer_type> args) override;
	virtual string_type toString() { return "<func | native | Sine>"; }
};

class Sqr : public BetaCallable
{
public:
	DEF_POINTER_TYPE(Sqr)
	DEF_IS_CONVERTIBLE_FROM(Sqr)

	virtual unsigned short arity() override { return 1; }
	virtual Token::pointer_type call(Interpreter * interpreter, std::list<Token::pointer_type> args) override;
	virtual string_type toString() { return "<func | native | Square Root>"; }
};

class Tan : public BetaCallable
{
public:
	DEF_POINTER_TYPE(Tan)
	DEF_IS_CONVERTIBLE_FROM(Tan)

	virtual unsigned short arity() override { return 1; }
	virtual Token::pointer_type call(Interpreter * interpreter, std::list<Token::pointer_type> args) override;
	virtual string_type toString() { return "<func | native | Tangent>"; }
};

class Pow : public BetaCallable
{
public:
	DEF_POINTER_TYPE(Pow)
	DEF_IS_CONVERTIBLE_FROM(Pow)

	virtual unsigned short arity() override { return 2; }
	virtual Token::pointer_type call(Interpreter * interpreter, std::list<Token::pointer_type> args) override;
	virtual string_type toString() { return "<func | native | Power>"; }
};