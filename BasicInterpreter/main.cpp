#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"
#include "resolver.hpp"
#include "interpreter.hpp"
#include "position_tracker.hpp"

using namespace std;

int main()
{
	try
	{
		PositionTracker::rawCode_ = "float x = abs(-1); word y = x;";
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