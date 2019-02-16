/**	@file tokenizer.cpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of Lexer class
*/

#include "lexer.hpp"
#include "word.hpp"
#include "boolean.hpp"
#include "float.hpp"
#include "variable.hpp"
#include "math_func.hpp"
#include "misc_func.hpp"
#include "operator.hpp"
#include "integer.hpp"
#include "keywords.hpp"
#include "word.hpp"

#include <exception>
#include <limits>
#include <sstream>
#include <stack>
#include <string>
#include <boost\math\special_functions\pow.hpp>
using namespace std;

/**
@name:		Lexer
@purpose:	defines a series of keywords, constants, and operators
@param:		null
@return:	Lexer
*/
Lexer::Lexer()
{
	// keywords
	specialTokens_["let"] = make<Let>();
	specialTokens_["shout"] = make<Print>();
	specialTokens_["if"] = make<If>();
	specialTokens_["else"] = make<Else>();
	specialTokens_["while"] = make<While>();
	specialTokens_["for"] = make<For>();
	specialTokens_["do"] = make<Do>();
	specialTokens_["func"] = make<Func>();
	specialTokens_["return"] = make<Return>();
	specialTokens_["class"] = make<Class>();
	specialTokens_["me"] = make<Me>();
	specialTokens_["inherits"] = make<Inherits>();
	specialTokens_["super"] = make<Super>();

	// constants
	specialTokens_["pi"] = make<Pi>();
	specialTokens_["e"] = make<E>();

	// special operators
	specialTokens_["and"] = make<And>();
	specialTokens_["nand"] = make<Nand>();
	specialTokens_["or"] = make<Or>();
	specialTokens_["nor"] = make<Nor>();
	specialTokens_["not"] = make<Not>();
	specialTokens_["xor"] = make<Xor>();
	specialTokens_["xnor"] = make<Xnor>();

	// boolean operands
	specialTokens_["true"] = make<True>();
	specialTokens_["false"] = make<False>();
}

/**
@name:		analyze
@purpose:	creates a series of tokens based on the characters of the input string
@param:		string_type const &
@return:	TokenList
*/
TokenList Lexer::analyze(string_type const & expression)
{
	TokenList tokens;
	auto currentChar = expression.cbegin();

	for (;;)
	{
		while (currentChar != end(expression) && (isspace(*currentChar) || *currentChar == '\n' || *currentChar == '\r'))
			++currentChar;

		if (currentChar == end(expression)) break;

		if (isdigit(*currentChar))
		{
			tokens.push_back(getNumber(currentChar, expression));
			continue;
		}

		if (isalpha(*currentChar))
		{
			tokens.push_back(getIdentifier(currentChar, expression));
			continue;
		}

		if (ispunct(*currentChar))
		{
			if (*currentChar == '=')
			{
				if (!tokens.empty())
				{
					if (is<Assignment>(tokens.back()))
					{
						tokens.pop_back();
						tokens.push_back(make<Equality>());
					}
					else if (is<Factorial>(tokens.back()))
					{
						tokens.pop_back();
						tokens.push_back(make<Inequality>());
					}
					else if (is<Greater>(tokens.back()))
					{
						tokens.pop_back();
						tokens.push_back(make<GreaterEqual>());
					}
					else if (is<Less>(tokens.back()))
					{
						tokens.pop_back();
						tokens.push_back(make<LessEqual>());
					}
					else
					{
						tokens.push_back(make<Assignment>());
					}

				}
				else
				{
					tokens.push_back(make<Assignment>());
				}
				++currentChar;
				continue;
			}

			if (*currentChar == '&')
			{
				if ((currentChar + 1) != end(expression))
				{
					if (*(++currentChar) == '&')
					{
						tokens.push_back(make<And>());
						++currentChar;
						continue;
					}
					else
						--currentChar;
				}
			}

			if (*currentChar == '|')
			{
				if ((currentChar + 1) != end(expression))
				{
					if (*(++currentChar) == '|')
					{
						tokens.push_back(make<Or>());
						++currentChar;
						continue;
					}
					else
						--currentChar;
				}
			}

			if (*currentChar == '(')
			{
				tokens.push_back(make<LeftBracket>());
				++currentChar;
				continue;
			}

			if (*currentChar == ')')
			{
				tokens.push_back(make<RightBracket>());
				++currentChar;
				continue;
			}

			if (*currentChar == '{')
			{
				tokens.push_back(make<LeftBrace>());
				++currentChar;
				continue;
			}

			if (*currentChar == '}')
			{
				tokens.push_back(make<RightBrace>());
				++currentChar;
				continue;
			}

			if (*currentChar == ';')
			{
				tokens.push_back(make<SemiColon>());
				++currentChar;
				continue;
			}

			if (*currentChar == ',')
			{
				tokens.push_back(make<Comma>());
				++currentChar;
				continue;
			}

			if (*currentChar == '.')
			{
				tokens.push_back(make<Dot>());
				++currentChar;
				continue;
			}

			if (*currentChar == '*')
			{
				tokens.push_back(make<Multiplication>());
				++currentChar;
				continue;
			}

			if (*currentChar == '/')
			{
				tokens.push_back(make<Division>());
				++currentChar;
				continue;
			}

			if (*currentChar == '!')
			{
				tokens.push_back(make<Factorial>());
				++currentChar;
				continue;
			}

			if (*currentChar == '^')
			{
				tokens.push_back(make<Power>());
				++currentChar;
				continue;
			}

			if (*currentChar == '>')
			{
				tokens.push_back(make<Greater>());
				++currentChar;
				continue;
			}

			if (*currentChar == '<')
			{
				tokens.push_back(make<Less>());
				++currentChar;
				continue;
			}

			if (*currentChar == ',')
			{
				if (is<Operand>(tokens.back()))
					tokens.push_back(make<ArgSeparator>());

				++currentChar;
				continue;
			}

			if (*currentChar == '+')
			{
				if (currentChar != expression.cbegin())
				{
					bool rPar = is<RightBracket>(tokens.back());
					bool oper = is<Operand>(tokens.back());
					bool post = is<Factorial>(tokens.back());

					if (rPar || oper || post)
						tokens.push_back(make<Addition>());
					else
						tokens.push_back(make<Identity>());
				}
				else
					tokens.push_back(make<Identity>());

				++currentChar;
				continue;
			}

			if (*currentChar == '-')
			{
				if (currentChar != expression.cbegin())
				{
					bool rPar = is<RightBracket>(tokens.back());
					bool oper = is<Operand>(tokens.back());
					bool post = is<Factorial>(tokens.back());

					if (rPar || oper || post)
						tokens.push_back(make<Subtraction>());
					else
						tokens.push_back(make<Negation>());
				}
				else
					tokens.push_back(make<Negation>());

				++currentChar;
				continue;
			}

			if (*currentChar == '"')
			{
				string_type word = "";
				bool closed = false;
				++currentChar;

				while (currentChar != end(expression))
				{
					if (*currentChar == '"')
					{
						closed = true;
						++currentChar;
						tokens.push_back(make<Word>(word));
						break;
					}

					word += *currentChar;
					++currentChar;
				}
					
				if (!closed)
					throw exception("Word has no closing \"");
				
				continue;
			}
		}

		throw BadCharException(expression, currentChar - begin(expression));
	}

	return tokens;
}

/**
@name:		getIdentifier
@purpose:	discerns whether a series of alphabetic characters is a keyword or a variable
@param:		Lexer::string_type::const_iterator &, Lexer::string_type const &
@return:	Lexer::pointer_type
*/
Token::pointer_type Lexer::getIdentifier(Lexer::string_type::const_iterator & curChar, Lexer::string_type const & expression)
{
	string_type ident;
	do
	{
		ident += *curChar++;
	} while (curChar != end(expression) && isalnum(*curChar));

	dictionary_type::iterator iter = specialTokens_.find(ident);
	if (iter != end(specialTokens_))
		return iter->second;
	else
	{
		return make<Variable>(ident);	
	}

	throw BadCharException(expression, curChar - begin(expression));
}

/**
@name:		getNumber
@purpose:	nuilds an Integer or Float token based on the input string
@param:		Lexer::string_type::const_iterator &, Lexer::string_type const &
@return:	Lexer::pointer_type
*/
Token::pointer_type Lexer::getNumber(Lexer::string_type::const_iterator & curChar, Lexer::string_type const & expression)
{
	using float_type = boost::multiprecision::number < boost::multiprecision::cpp_dec_float<1000, int32_t, void> >;
	using int_type = boost::multiprecision::cpp_int;

	auto currentCharNum = curChar;
	int_type acc = boost::lexical_cast<int_type>(*currentCharNum);
	++curChar;

	if (curChar != expression.cend())
	{
		while (isdigit(*curChar))
		{
			currentCharNum = curChar;
			acc *= 10;
			acc += boost::lexical_cast<int_type>(*currentCharNum);
			++curChar;
			if (curChar == expression.cend())
				return make<Integer>(acc);
		}
	}
	else
	{
		return make<Integer>(acc);
	}

	if (*curChar == '.')
	{
		float_type accR = boost::lexical_cast<float_type>(acc);
		int_type div = 1;
		int_type base = 10;
		++curChar;

		if (curChar != expression.cend())
		{
			while (isdigit(*curChar))
			{
				currentCharNum = curChar;
				int_type power = 1;

				for (int_type i = 0; i < div; ++i)
					power *= base;

				accR += boost::lexical_cast<float_type>(*currentCharNum) / boost::lexical_cast<float_type>(power);
				++div;
				++curChar;
				if (curChar == expression.cend())
					break;
			}
			return make<Float>(accR);
		}
		else
		{
			return make<Float>(accR);
		}
	}

	return make<Integer>(acc);
}
