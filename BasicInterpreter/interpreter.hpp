/**	@file intepreter.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif defines the structure of the language's intepreter
*/

#pragma once
#include "visitor.hpp"
#include "environment.hpp"
#include <list>
#include <map>

class Interpreter : public ExprVisitor, public StmtVisitor
{
public:
	class InterpreterException : public std::exception
	{
	public:
		InterpreterException(char const* msg) : std::exception(msg) {}
	};
public:
	using oper_type = Operand::pointer_type;
	using tok_type = Token::pointer_type;
	using expr_type = Expression::expr_type;
	using stmt_p = Statement::pointer_type;
	using stmt_list = std::list<stmt_p>;
private:
	std::shared_ptr<Environment> env_;
	std::shared_ptr<Environment> globals_;
	std::map<Expression *, unsigned> locals_;
public:
	Interpreter();

	virtual Token::pointer_type visit(AssignExpression * expr) override;
	virtual Token::pointer_type visit(BinaryExpression * expr) override;
	virtual Token::pointer_type visit(CallExpression * expr) override;
	virtual Token::pointer_type visit(UnaryExpression * expr) override;
	virtual Token::pointer_type visit(GroupExpression * expr) override;
	virtual Token::pointer_type visit(LiteralExpression * expr) override;
	virtual Token::pointer_type visit(VariableExpression * expr) override;
	virtual Token::pointer_type visit(LogicalExpression * expr) override;
	virtual Token::pointer_type visit(GetExpression * expr) override;
	virtual Token::pointer_type visit(SetExpression * expr) override;
	virtual Token::pointer_type visit(MeExpression * expr) override;
	virtual Token::pointer_type visit(SuperExpression * expr) override;
	
	virtual void visit(StmtExpression * expr) override;
	virtual void visit(StmtPrint * expr) override;
	virtual void visit(StmtReturn * expr) override;
	virtual void visit(StmtVariable * expr) override;
	virtual void visit(StmtBlock * expr) override;
	virtual void visit(StmtClass * expr) override;
	virtual void visit(StmtIf * expr) override;
	virtual void visit(StmtWhile * expr) override;
	virtual void visit(StmtDoWhile * expr) override;
	virtual void visit(StmtFunc * expr) override;
	
	void interpret(stmt_list lstState);
	void executeBlock(stmt_list statements, std::shared_ptr<Environment> env);
	void resolve(Expression * expr, unsigned depth);

private:
	void prepareNativeFunctions();
	tok_type evaluate(expr_type expr);
	void execute(stmt_p stmt);
	Token::pointer_type lookUpVariable(std::string name, Expression * expr);
	std::string VarTypeName(Variable::VarType type);
	bool checkType(Variable::VarType type, Token::pointer_type val);

	friend class CustomFunction;
};