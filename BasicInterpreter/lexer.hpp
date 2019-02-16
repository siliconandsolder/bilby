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

class Lexer
{
public:
	using string_type = std::string;

	class LexerException : public std::exception
	{
		string_type expression_;
		unsigned location_;
	public:
		LexerException(string_type const& expression, unsigned location, char const * msg): std::exception(msg), expression_(expression), location_(location) {}
	
		string_type getExpression() const { return expression_; }

		unsigned getLocation() const { return location_; }
	};

	class BadCharException : public LexerException
	{
	public:
		BadCharException(string_type const& expression, unsigned location) : LexerException(expression, location, "Bad Character in expression!") {}
	};

private:
	using dictionary_type = std::map<string_type, Token::pointer_type>;

	dictionary_type specialTokens_;
	dictionary_type variables_;

public:
	Lexer();
	TokenList analyze(string_type const& expression);

private:
	Token::pointer_type getIdentifier(Lexer::string_type::const_iterator& curChar, Lexer::string_type const& expression);
	Token::pointer_type getNumber(Lexer::string_type::const_iterator& curChar, Lexer::string_type const& expression);
};