#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "visitor.hpp"


class AstPrinter : public ExprVisitor
{
private:
	std::string msg_;
	std::stringstream ss_;
public:
	std::string print(Expression::expr_type exp);

	virtual Token::pointer_type visit(BinaryExpression * expr) override;
	virtual Token::pointer_type visit(UnaryExpression * expr) override;
	virtual Token::pointer_type visit(GroupExpression * expr) override;
	virtual Token::pointer_type visit(LiteralExpression * expr) override;
	virtual Token::pointer_type visit(VariableExpression * expr) override;

private:
	void parenthesize(std::string name, std::vector<Expression::expr_type> exprs);
};