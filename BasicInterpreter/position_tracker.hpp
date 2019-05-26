/**	@file position_tracker.hpp
	@author Benjamin Godin
	@date 2019-05-01
	@version 1.0.0
	@note Developed for C++17
	@breif tracks the progress of the interpreter as it lexes, parses, resolves, and interprets the code
*/

#pragma once
#include <ostream>
#include "statement.hpp"

enum Stage : short
{
	RAW = 0,
	LEXED,
	PARSED
};

class PositionTracker
{
public:
	using token_tracker = std::list<std::pair<unsigned, Token::pointer_type>>;
	using token_iterator = token_tracker::const_iterator;

	using stmt_tracker = std::list<std::pair<unsigned, Statement::pointer_type>>;
	using stmt_iterator = stmt_tracker::const_iterator;

	// Token, line number, indent, character position
	static token_tracker tokenPosition_;
	static token_iterator itTokPos_;

	static stmt_tracker stmtPosition_;
	static stmt_iterator itStmtPos_;
	static Expression::pointer_type lastExp_;
	static Token::pointer_type lastTok_;
	// used to find the correct position in the code string
	static unsigned cursorPosition_;
	// holds the code as it was written
	static std::string rawCode_;
	static short stage_;

	static std::string printPosition();
	static void addToken(std::shared_ptr<Token> token);
	static void addStatement(Statement::pointer_type stmt);
	/*static void getStmtPositionResolver(Statement::pointer_type stmt, unsigned startPos);
	static void getStmtPositionInterpreter(Statement::pointer_type stmt, unsigned startPos);
	static unsigned calculateExpOffsetResolver(Expression::pointer_type exp, unsigned startPos);
	static unsigned calculateExpOffsetInterpreter(Expression::pointer_type exp, unsigned startPos);*/
};