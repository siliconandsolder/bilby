/**	@file visitor.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif visitor classes for expressions and statements (Visitor Pattern)
*/

#pragma once
#include "statement.hpp"

class ExprVisitor
{
public:
	virtual Token::pointer_type visit(AssignExpression * expr) = 0;
	virtual Token::pointer_type visit(BinaryExpression * expr) = 0;
	virtual Token::pointer_type visit(CallExpression * expr) = 0;
	virtual Token::pointer_type visit(UnaryExpression * expr) = 0;
	virtual Token::pointer_type visit(GroupExpression * expr) = 0;
	virtual Token::pointer_type visit(LiteralExpression * expr) = 0;
	virtual Token::pointer_type visit(LogicalExpression * expr) = 0;
	virtual Token::pointer_type visit(VariableExpression * expr) = 0;
	virtual Token::pointer_type visit(GetExpression * expr) = 0;
	virtual Token::pointer_type visit(SetExpression * expr) = 0;
	virtual Token::pointer_type visit(MeExpression * expr) = 0;
	virtual Token::pointer_type visit(SuperExpression * expr) = 0;
};

class StmtVisitor
{
public:
	virtual void visit(StmtExpression * expr) = 0;
	virtual void visit(StmtFunc * expr) = 0;
	virtual void visit(StmtPrint * expr) = 0;
	virtual void visit(StmtReturn * expr) = 0;
	virtual void visit(StmtVariable * expr) = 0;
	virtual void visit(StmtBlock * expr) = 0;
	virtual void visit(StmtClass * expr) = 0;
	virtual void visit(StmtIf * expr) = 0;
	virtual void visit(StmtWhile * expr) = 0;
	virtual void visit(StmtDoWhile * expr) = 0;
};