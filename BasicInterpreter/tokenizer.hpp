/**	@file tokenizer.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif converts string input into a list of tokens
*/

#pragma once

#include "token.hpp"
#include "variable.hpp"
#include "operand.hpp"
#include <map>
#include <string>

class Tokenizer
{
public:
	using string_type = std::string;

	class TokenException : public std::exception
	{
		string_type expression_;
		unsigned location_;
	public:
		TokenException(string_type const& expression, unsigned location, char const * msg): std::exception(msg), expression_(expression), location_(location) {}
	
		string_type getExpression() const { return expression_; }

		unsigned getLocation() const { return location_; }
	};

	class BadCharException : public TokenException
	{
	public:
		BadCharException(string_type const& expression, unsigned location) : TokenException(expression, location, "Bad Character in expression!") {}
	};

	class BadFuncException : public TokenException
	{
	public:
		BadFuncException(string_type const& expression, unsigned location) : TokenException(expression, location, "Bad Character in expression!") {}
	};

	class NumOverflowException : public TokenException
	{
	public:
		NumOverflowException(string_type const& expression, unsigned location) : TokenException(expression, location, "Bad Character in expression!") {}
	};

private:
	using dictionary_type = std::map<string_type, Token::pointer_type>;

	dictionary_type specialTokens_;
	dictionary_type variables_;

public:
	Tokenizer();
	TokenList tokenize(string_type const& expression);

private:
	Token::pointer_type getIdentifier(Tokenizer::string_type::const_iterator& curChar, Tokenizer::string_type const& expression);
	Token::pointer_type getNumber(Tokenizer::string_type::const_iterator& curChar, Tokenizer::string_type const& expression);
};