/**	@file position_tracker.cpp
	@author Benjamin Godin
	@date 2019-05-01
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of class PositionTracker
*/

#include <iterator>
#include "position_tracker.hpp"

using namespace std;

PositionTracker::token_tracker PositionTracker::tokenPosition_;
PositionTracker::token_iterator PositionTracker::itTokPos_;
PositionTracker::stmt_tracker PositionTracker::stmtPosition_;
PositionTracker::stmt_iterator PositionTracker::itStmtPos_;
Expression::pointer_type PositionTracker::lastExp_;
Token::pointer_type PositionTracker::lastTok_;
unsigned PositionTracker::cursorPosition_;
string PositionTracker::rawCode_;
short PositionTracker::stage_;

/**
@name:		printPosition
@purpose:	prints the line of code where an exception was thrown
@param:		void
@return:	std::string
*/
std::string PositionTracker::printPosition()
{
	ostringstream oss;
	string errorType = "Syntax Error:";
	unsigned line = 1, indent = 0;

	// we're in the parser
	if (stage_ == Stage::LEXED)
	{
		errorType = "Parsing Error:";
		cursorPosition_ = itTokPos_->first;
	}
	else if (stage_ == Stage::PARSED)	// we're in the resolver or interpreter
	{
		errorType = "Runtime Error:";
		cursorPosition_ = itStmtPos_->first;
	}

	auto itEndChar = rawCode_.cbegin() + cursorPosition_ + 1;

	// find how many lines and indents there are between the beginning of the code and the current position
	for (auto itCurChar = rawCode_.cbegin(); itCurChar != itEndChar; ++itCurChar)
	{
		if (*itCurChar == '\n' || *itCurChar == '\r')
		{
			++line;
			indent = 1;
		}
		else
			++indent;
	}

	// find last newline
	auto itlastLine = rawCode_.cbegin() + cursorPosition_, itNextLine = rawCode_.cbegin() + cursorPosition_;

	while (itlastLine != rawCode_.cbegin() && (*(itlastLine - 1) != '\n' && *(itlastLine - 1) != '\r'))
		--itlastLine;

	// find next newline
	while (itNextLine != rawCode_.cend() && (*itNextLine != '\n' && *itNextLine != '\r'))
		++itNextLine;

	// print line of exception
	oss << errorType << " line " << line << ", indent " << indent << '\n';
	oss << string(itlastLine, itNextLine) << '\n';
	oss << string(distance(rawCode_.cbegin(), rawCode_.cbegin() + cursorPosition_), '-');
	oss << '^';
	oss << string(distance(rawCode_.cbegin() + cursorPosition_ + 1, rawCode_.cend()), '-');
	oss << '\n';

	return oss.str();
}

/**
@name:		addToken
@purpose:	adds a token and its position in the code into tokenPosition_
@param:		std::shared_ptr<Token>
@return:	void
*/
void PositionTracker::addToken(std::shared_ptr<Token> token)
{
	tokenPosition_.push_back(pair<unsigned, Token::pointer_type>(cursorPosition_, token));
}

/**
@name:		addStatement
@purpose:	adds a statement and its position in the code into stmtPosition_
@param:		Statement::pointer_type
@return:	void
*/
void PositionTracker::addStatement(Statement::pointer_type stmt)
{
	stmtPosition_.push_back(pair<unsigned, Statement::pointer_type>(cursorPosition_, stmt));
	
	// don't try to reassign cursorPosition_ if we've reached the end of the token list
	if (itTokPos_ != tokenPosition_.cend())
		cursorPosition_ = itTokPos_->first;
}