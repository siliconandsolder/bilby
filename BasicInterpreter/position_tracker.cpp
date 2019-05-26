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

//void PositionTracker::getStmtPositionResolver(Statement::pointer_type stmt, unsigned startPos)
//{
//	
//	if (is<StmtBlock>(stmt))
//	{
//		StmtBlock::pointer_type block = convert<StmtBlock>(stmt);
//		for (auto state : block->statements_)
//		{
//			getStmtPositionResolver(state, startPos);
//			if (foundExp_)
//				return;
//		}
//	}
//	else if (is<StmtExpression>(stmt))
//	{
//		StmtExpression::pointer_type exp = convert<StmtExpression>(stmt);
//		cursorPosition_ = calculateExpOffsetResolver(exp->expr_, PositionTracker::itStmtPos_->second);
//		return;
//	}
//	else if (is<StmtFunc>(stmt))
//	{
//		StmtFunc::pointer_type func = convert<StmtFunc>(stmt);
//		if (PositionTracker::lastTok_ == func->ident_)
//		{
//			cursorPosition_ = PositionTracker::rawCode_.find(func->ident_->toString(), PositionTracker::itStmtPos_->second);
//			foundExp_ = true;
//			return;
//		}
//		else 
//		{
//			for (auto tok : func->params_)
//			{
//				if (PositionTracker::lastTok_ == tok)
//				{
//					cursorPosition_ = PositionTracker::rawCode_.find(tok->toString(), PositionTracker::itStmtPos_->second);
//					foundExp_ = true;
//					return;
//				}
//			}
//
//			for (auto state : func->body_)
//			{
//				getStmtPositionResolver(state, startPos);
//				if (foundExp_)
//					return;
//			}
//		}
//	}
//	else if (is<StmtClass>(stmt))
//	{
//		StmtClass::pointer_type clas = convert<StmtClass>(stmt);
//
//		if (PositionTracker::lastTok_ == clas->name_)
//		{
//			// actually, get the position of the current statement
//			cursorPosition_ = PositionTracker::rawCode_.find(clas->name_->toString(), PositionTracker::itStmtPos_->second);
//			foundExp_ = true;
//		}
//		else if (lastExp_ == clas->super_)
//		{
//			cursorPosition_ = calculateExpOffsetResolver(lastExp_, startPos);
//			foundExp_ = true;
//		}
//		else
//			for (auto state : clas->methods_)
//			{
//				getStmtPositionResolver(state, startPos);
//				if (foundExp_)
//					return;
//			}
//	}
//	else if (is<StmtIf>(stmt))
//	{
//		StmtIf::pointer_type ifStmt = convert<StmtIf>(stmt);
//		cursorPosition_ = calculateExpOffsetResolver(ifStmt->expr_, startPos);
//		foundExp_ = true;
//	}
//	else if (is<StmtWhile>(stmt))
//	{
//		StmtWhile::pointer_type whileStmt = convert<StmtWhile>(stmt);
//		if (lastExp_ == whileStmt->expr_)
//		{
//			cursorPosition_ = calculateExpOffsetResolver(lastExp_, startPos);
//			foundExp_ = true;
//		}
//		else
//			getStmtPositionResolver(whileStmt->whileBlock_, startPos);
//	}
//	else if (is<StmtDoWhile>(stmt))
//	{
//		StmtDoWhile::pointer_type whileStmt = convert<StmtDoWhile>(stmt);
//		if (lastExp_ == whileStmt->expr_)
//		{
//			cursorPosition_ = calculateExpOffsetResolver(lastExp_, startPos);
//			foundExp_ = true;
//		}
//		else
//			getStmtPositionResolver(whileStmt->whileBlock_, startPos);
//	}
//	else if (is<StmtPrint>(stmt))
//	{
//		StmtPrint::pointer_type printStmt = convert<StmtPrint>(stmt);
//		cursorPosition_ = calculateExpOffsetResolver(printStmt->expr_, startPos);
//		foundExp_ = true;
//	}
//	else if (is<StmtReturn>(stmt))
//	{
//		StmtReturn::pointer_type retStmt = convert<StmtReturn>(stmt);
//		cursorPosition_ = cursorPosition_ = PositionTracker::rawCode_.find("return", PositionTracker::itStmtPos_->second);
//		foundExp_ = true;
//	}
//	else if (is<StmtVariable>(stmt))
//	{
//		StmtVariable::pointer_type varStmt = convert<StmtVariable>(stmt);
//		cursorPosition_ = calculateExpOffsetResolver(varStmt->expr_, startPos);
//		foundExp_ = true;
//	}
//}
//
//void PositionTracker::getStmtPositionInterpreter(Statement::pointer_type stmt, unsigned startPos)
//{
//	if (is<StmtBlock>(stmt))
//	{
//		StmtBlock::pointer_type block = convert<StmtBlock>(stmt);
//		for (auto state : block->statements_)
//		{
//			getStmtPositionInterpreter(state, startPos);
//			if (foundExp_)
//				return;
//		}
//	}
//	else if (is<StmtExpression>(stmt))
//	{
//		StmtExpression::pointer_type exp = convert<StmtExpression>(stmt);
//		if (lastExp_ == exp->expr_)
//		{
//			expStr_ = exp->expr_->toString();
//		}
//	}
//	else if (is<StmtFunc>(stmt))
//	{
//		StmtFunc::pointer_type func = convert<StmtFunc>(stmt);
//
//	}
//	else if (is<StmtClass>(stmt))
//	{
//		StmtClass::pointer_type clas = convert<StmtClass>(stmt);
//		if (lastExp_ == clas->super_)
//		{
//			cursorPosition_ = calculateExpOffsetInterpreter(lastExp_, startPos);
//			foundExp_ = true;
//		}
//		else
//			for (auto state : clas->methods_)
//			{
//				getStmtPositionInterpreter(state, startPos);
//				if (foundExp_)
//					return;
//			}
//	}
//	else if (is<StmtIf>(stmt))
//	{
//		StmtIf::pointer_type ifStmt = convert<StmtIf>(stmt);
//		cursorPosition_ = calculateExpOffsetInterpreter(ifStmt->expr_, startPos);
//		foundExp_ = true;
//	}
//	else if (is<StmtWhile>(stmt))
//	{
//		StmtWhile::pointer_type whileStmt = convert<StmtWhile>(stmt);
//		if (lastExp_ == whileStmt->expr_)
//		{
//			cursorPosition_ = calculateExpOffsetInterpreter(lastExp_, startPos);
//			foundExp_ = true;
//		}
//		else
//			getStmtPositionInterpreter(whileStmt->whileBlock_, startPos);
//	}
//	else if (is<StmtDoWhile>(stmt))
//	{
//		StmtDoWhile::pointer_type whileStmt = convert<StmtDoWhile>(stmt);
//		if (lastExp_ == whileStmt->expr_)
//		{
//			cursorPosition_ = calculateExpOffsetInterpreter(lastExp_, startPos);
//			foundExp_ = true;
//		}
//		else
//			getStmtPositionInterpreter(whileStmt->whileBlock_, startPos);
//	}
//	else if (is<StmtPrint>(stmt))
//	{
//		StmtPrint::pointer_type printStmt = convert<StmtPrint>(stmt);
//		cursorPosition_ = calculateExpOffsetInterpreter(printStmt->expr_, startPos);
//		foundExp_ = true;
//	}
//	else if (is<StmtReturn>(stmt))
//	{
//		StmtReturn::pointer_type retStmt = convert<StmtReturn>(stmt);
//		cursorPosition_ = calculateExpOffsetInterpreter(retStmt->expr_, startPos);
//		foundExp_ = true;
//	}
//	else if (is<StmtVariable>(stmt))
//	{
//		StmtVariable::pointer_type varStmt = convert<StmtVariable>(stmt);
//		cursorPosition_ = calculateExpOffsetInterpreter(varStmt->expr_, startPos);
//		foundExp_ = true;
//	}
//}
//
//unsigned PositionTracker::calculateExpOffsetResolver(Expression::pointer_type exp, unsigned startPos)
//{
//	if (is<VariableExpression>(exp))
//	{
//		
//	}
//	else if (is<MeExpression>(exp))
//	{
//		
//	}
//	else if (is<SuperExpression>(exp))
//	{
//		
//	}
//}
//
//unsigned PositionTracker::calculateExpOffsetInterpreter(Expression::pointer_type exp, unsigned startPos)
//{
//	return 0;
//}
