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
#include "position_tracker.hpp"
#include <iostream>
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
		PositionTracker::rawCode_ = text;
		PositionTracker::cursorPosition_ = 0;
		PositionTracker::stage_ = Stage::RAW;

		Lexer lex;
		TokenList list = lex.analyze(PositionTracker::rawCode_);

		PositionTracker::stage_ = Stage::LEXED;
		PositionTracker::itTokPos_ = PositionTracker::tokenPosition_.cbegin();
		PositionTracker::cursorPosition_ = 0;

		Parser parse(list);
		Parser::stmt_list stmts = parse.parse();

		PositionTracker::itStmtPos_ = PositionTracker::stmtPosition_.cbegin();
		PositionTracker::stage_ = Stage::PARSED;

		Interpreter interpreter;
		Resolver resolver(&interpreter);
		resolver.resolve(stmts);

		PositionTracker::itStmtPos_ = PositionTracker::stmtPosition_.cbegin();

		interpreter.interpret(stmts);
	}
	catch (exception& ex)
	{
		cerr << ex.what() << endl;
		cerr << PositionTracker::printPosition() << endl;
	}
}
