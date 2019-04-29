/**	@file entry.cpp
	@author Benjamin Godin
	@date 2019-03-13
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of Entry class, an entry-point for the Wrapper class
*/

#include "entry.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include "interpreter.hpp"
#include "resolver.hpp"
#include <iostream>
#include <consoleapi.h>
using namespace std;

/**
@name:		interpret
@purpose:	lexes, parses, and interprets the provided string argument
@param:		std::string
@return:	void
*/
void Entry::interpret(std::string text)
{
	try
	{
		Lexer lex;
		TokenList tokens = lex.analyze(text);

		Parser p(tokens);
		Parser::stmt_list lst = p.parse();

		Interpreter it;

		Resolver res(&it);
		res.resolve(lst);

		it.interpret(lst);
	}
	catch (exception& ex)
	{
		cout << ex.what() << endl;
	}
}

void Entry::setStdHandle(HANDLE handle)
{
	SetStdHandle(STD_OUTPUT_HANDLE, handle);
}
