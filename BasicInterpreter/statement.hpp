/**	@file statement.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif defines all possible statements, which are a collection of one or more expressions
*/

#pragma once
#include "expression.hpp"
#include "variable.hpp"
#include "keywords.hpp"
#include <list>

class StmtVisitor;

class Statement : public Token
{
public:
	DEF_POINTER_TYPE(Statement)
	DEF_IS_CONVERTIBLE_FROM(Statement)
	using stmt_p = Statement::pointer_type;
	virtual ~Statement() {}
	virtual void accept(StmtVisitor &v) = 0;
};

class StmtBlock : public Statement
{
public:
	DEF_POINTER_TYPE(StmtBlock)
	DEF_IS_CONVERTIBLE_FROM(StmtBlock)
public:
	std::list<Statement::pointer_type> statements_;
public:
	StmtBlock(std::list<Statement::pointer_type> stats) : statements_(stats) {}

	virtual void accept(StmtVisitor &v);
};

class StmtExpression : public Statement
{
public:
	DEF_POINTER_TYPE(StmtExpression)
		DEF_IS_CONVERTIBLE_FROM(StmtExpression)
public:
	Expression::expr_type expr_;
public:
	StmtExpression(Expression::expr_type expr) : expr_(expr) {}

	virtual void accept(StmtVisitor &v);
};

class StmtFunc : public Statement
{
public:
	DEF_POINTER_TYPE(StmtFunc)
	DEF_IS_CONVERTIBLE_FROM(StmtFunc)
public:
	Variable::pointer_type ident_;
	VarType retType_;
	std::list<Token::pointer_type> params_;
	std::list<Statement::pointer_type> body_;
public:
	StmtFunc(Variable::pointer_type ident, std::list<Token::pointer_type> params, std::list<Statement::pointer_type> body) 
				: ident_(ident), params_(params), body_(body) {}

	virtual void accept(StmtVisitor &v);
};

class StmtClass : public Statement
{
public:
	DEF_POINTER_TYPE(StmtClass)
		DEF_IS_CONVERTIBLE_FROM(StmtClass)
public:
	Variable::pointer_type name_;
	VariableExpression::pointer_type super_;
	std::list<Variable::pointer_type> data_;
	std::list<StmtFunc::pointer_type> methods_;
public:
	StmtClass(Variable::pointer_type name, VariableExpression::pointer_type super, std::list<Variable::pointer_type> data, std::list<StmtFunc::pointer_type> methods) : name_(name), super_(super), data_(data), methods_(methods) {}

	virtual void accept(StmtVisitor &v);
};

class StmtIf : public Statement
{
public:
	DEF_POINTER_TYPE(StmtExpression)
		DEF_IS_CONVERTIBLE_FROM(StmtExpression)
public:
	Expression::expr_type expr_;
	stmt_p thenBlock_;
	stmt_p elseBlock_;
public:
	StmtIf(Expression::expr_type expr, stmt_p thenBlock, stmt_p elseBlock) : expr_(expr), thenBlock_(thenBlock), elseBlock_(elseBlock) {}

	virtual void accept(StmtVisitor &v);
};

class StmtWhile : public Statement
{
public:
	DEF_POINTER_TYPE(StmtWhile)
		DEF_IS_CONVERTIBLE_FROM(StmtWhile)
public:
	Expression::expr_type expr_;
	stmt_p whileBlock_;
public:
	StmtWhile(Expression::expr_type expr, stmt_p whileBlock) : expr_(expr), whileBlock_(whileBlock) {}

	virtual void accept(StmtVisitor &v);
};

class StmtDoWhile : public Statement
{
public:
	DEF_POINTER_TYPE(StmtDoWhile)
		DEF_IS_CONVERTIBLE_FROM(StmtDoWhile)
public:
	Expression::expr_type expr_;
	stmt_p whileBlock_;
public:
	StmtDoWhile(Expression::expr_type expr, stmt_p whileBlock) : expr_(expr), whileBlock_(whileBlock) {}

	virtual void accept(StmtVisitor &v);
};

class StmtPrint : public Statement
{
public:
	DEF_POINTER_TYPE(StmtPrint)
		DEF_IS_CONVERTIBLE_FROM(StmtPrint)
public:
	Expression::expr_type expr_;
public:
	StmtPrint(Expression::expr_type expr) : expr_(expr) {}

	virtual void accept(StmtVisitor &v);
};

class StmtReturn : public Statement
{
public:
	DEF_POINTER_TYPE(StmtReturn)
		DEF_IS_CONVERTIBLE_FROM(StmtReturn)
public:
	Return::pointer_type ret_;
	Expression::expr_type expr_;
public:
	StmtReturn(Return::pointer_type ret, Expression::expr_type expr) : ret_(ret), expr_(expr) {}

	virtual void accept(StmtVisitor &v);
};

class StmtVariable : public Statement
{
public:
	DEF_POINTER_TYPE(StmtPrint)
		DEF_IS_CONVERTIBLE_FROM(StmtPrint)
public:
	Variable::pointer_type var_;
	Expression::expr_type expr_;

	StmtVariable(Variable::pointer_type var, Expression::expr_type expr) : var_(var), expr_(expr) {}

	virtual void accept(StmtVisitor &v);
};