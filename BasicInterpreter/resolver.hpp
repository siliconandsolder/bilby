/**	@file resolver.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif defines the structure of the resolver, which ensures that all variable declarations are 
			legal and recorded
*/

#pragma once
#include "interpreter.hpp"
#include <list>
#include <stack>
#include <string>

class Resolver : public ExprVisitor, public StmtVisitor
{
public:
	class ResolverException : public std::exception
	{
	public:
		ResolverException(std::string msg) : std::exception(msg.c_str()) {}
	};
private:
	enum class FuncType { NONE, FUNC, INIT, METHOD };
	enum class ClassType { NONE, CLASS, SUBCLASS };
	FuncType curFunc_;
	ClassType curClass_;
	std::vector<std::map<std::string, bool>> scopes_;
	Interpreter * interpreter_;

public:
	Resolver(Interpreter * interpreter);

private:
	void resolve(Statement::pointer_type statement);
	void beginScope();
	void endScope();
	void resolve(std::shared_ptr<Expression> expr);
	void declare(Token::pointer_type token);
	void define(Token::pointer_type token);
	void resolveLocal(Expression * expr, Token::pointer_type token);
	void resolveFunction(StmtFunc * func, FuncType type);

public:
	void resolve(std::list<Statement::pointer_type> statements);

	virtual Token::pointer_type visit(AssignExpression * expr) override;
	virtual Token::pointer_type visit(BinaryExpression * expr) override;
	virtual Token::pointer_type visit(CallExpression * expr) override;
	virtual Token::pointer_type visit(UnaryExpression * expr) override;
	virtual Token::pointer_type visit(GroupExpression * expr) override;
	virtual Token::pointer_type visit(LiteralExpression * expr) override;
	virtual Token::pointer_type visit(LogicalExpression * expr) override;
	virtual Token::pointer_type visit(VariableExpression * expr) override;
	virtual Token::pointer_type visit(GetExpression * expr) override;
	virtual Token::pointer_type visit(SetExpression * expr) override;
	virtual Token::pointer_type visit(MeExpression * expr) override;
	virtual Token::pointer_type visit(SuperExpression * expr) override;

	virtual void visit(StmtExpression * expr) override;
	virtual void visit(StmtFunc * expr) override;
	virtual void visit(StmtPrint * expr) override;
	virtual void visit(StmtReturn * expr) override;
	virtual void visit(StmtVariable * expr) override;
	virtual void visit(StmtBlock * expr) override;
	virtual void visit(StmtClass * expr) override;
	virtual void visit(StmtIf * expr) override;
	virtual void visit(StmtWhile * expr) override;
	virtual void visit(StmtDoWhile * expr) override;
};