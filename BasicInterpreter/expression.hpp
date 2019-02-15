#pragma once
#include "operator.hpp"
#include "variable.hpp"
#include <list>

class ExprVisitor;

class Expression : public Token 
{
public:
	DEF_POINTER_TYPE(Expression)
	DEF_IS_CONVERTIBLE_FROM(Expression)
	using expr_type = std::shared_ptr<Expression>;
	virtual ~Expression() {}

	virtual Token::pointer_type accept(ExprVisitor &v) = 0;
};

class AssignExpression : public Expression
{
public:
	DEF_POINTER_TYPE(AssignExpression)
		DEF_IS_CONVERTIBLE_FROM(AssignExpression)
		using var_type = Variable::pointer_type;
public:
	var_type oper_;
	expr_type expr_;
public:
	AssignExpression() = delete;
	AssignExpression(var_type oper, expr_type expr) : oper_(oper), expr_(expr) {}
	var_type getValue() { return oper_; }

	Token::pointer_type accept(ExprVisitor &v) override final;
};

class BinaryExpression : public Expression
{
public:
	DEF_POINTER_TYPE(BinaryExpression)
		DEF_IS_CONVERTIBLE_FROM(BinaryExpression)
	using oper_type = std::shared_ptr<BinaryOperator>;
public:
	expr_type left_;
	oper_type oper_;
	expr_type right_;
public:
	BinaryExpression() = delete;
	BinaryExpression(expr_type left, oper_type oper, expr_type right) : left_(left), oper_(oper), right_(right) {}
	
	Token::pointer_type accept(ExprVisitor &v) override final;
};

class SetExpression : public Expression
{
public:
	DEF_POINTER_TYPE(SetExpression)
	DEF_IS_CONVERTIBLE_FROM(SetExpression)
public:
	expr_type obj_;
	Variable::pointer_type name_;
	expr_type val_;
public:
	SetExpression() = delete;
	SetExpression(expr_type obj, Variable::pointer_type name, expr_type val) : obj_(obj), name_(name), val_(val) {}

	Token::pointer_type accept(ExprVisitor &v) override final;
};

class MeExpression : public Expression
{
public:
	DEF_POINTER_TYPE(MeExpression)
		DEF_IS_CONVERTIBLE_FROM(MeExpression)
public:
	Variable::pointer_type keyword_;
public:
	MeExpression() = delete;
	MeExpression(Variable::pointer_type keyword) : keyword_(keyword) {}

	Token::pointer_type accept(ExprVisitor &v) override final;
};

class SuperExpression : public Expression
{
public:
	DEF_POINTER_TYPE(SuperExpression)
	DEF_IS_CONVERTIBLE_FROM(SuperExpression)
public:
	Variable::pointer_type keyword_;
	Variable::pointer_type method_;
public:
	SuperExpression() = delete;
	SuperExpression(Variable::pointer_type keyword, Variable::pointer_type method) : keyword_(keyword), method_(method) {}

	Token::pointer_type accept(ExprVisitor &v) override final;
};

class CallExpression : public Expression
{
public:
	DEF_POINTER_TYPE(CallExpression)
		DEF_IS_CONVERTIBLE_FROM(CallExpression)
		using paren_type = std::shared_ptr<RightBracket>;
public:
	expr_type callee_;
	paren_type paren_;
	std::list<expr_type> args_;
public:
	CallExpression() = delete;
	CallExpression(expr_type callee, paren_type paren, std::list<expr_type> args) : callee_(callee), paren_(paren), args_(args) {}

	Token::pointer_type accept(ExprVisitor &v) override final;
};

class GetExpression : public Expression
{
public:
	DEF_POINTER_TYPE(GetExpression)
	DEF_IS_CONVERTIBLE_FROM(GetExpression)
public:
	expr_type obj_;
	Variable::pointer_type name_;
public:
	GetExpression() = delete;
	GetExpression(expr_type obj, Variable::pointer_type name) : obj_(obj), name_(name) {}

	Token::pointer_type accept(ExprVisitor &v) override final;
};

class UnaryExpression : public Expression
{
public:
	DEF_POINTER_TYPE(UnaryExpression)
		DEF_IS_CONVERTIBLE_FROM(UnaryExpression)
	using oper_type = std::shared_ptr<UnaryOperator>;
public:
	oper_type oper_;
	expr_type exp_;
public:
	UnaryExpression() = delete;
	UnaryExpression(oper_type oper, expr_type exp) : oper_(oper), exp_(exp) {}

	Token::pointer_type accept(ExprVisitor &v) override final;
};

class GroupExpression : public Expression
{
public:
	DEF_POINTER_TYPE(GroupExpression)
		DEF_IS_CONVERTIBLE_FROM(GroupExpression)
	expr_type expr_;
	std::shared_ptr<LeftBracket> left_;
	std::shared_ptr<RightBracket> right_;
public:
	GroupExpression() = delete;
	GroupExpression(expr_type expr): left_(new LeftBracket()), expr_(expr), right_(new RightBracket()) {}

	Token::pointer_type accept(ExprVisitor &v) override final;
};

class LiteralExpression : public Expression
{
public:
	DEF_POINTER_TYPE(LiteralExpression)
		DEF_IS_CONVERTIBLE_FROM(LiteralExpression)
	using oper_type = std::shared_ptr<Operand>;
public:
	oper_type oper_;
public:
	LiteralExpression() = delete;
	LiteralExpression(oper_type oper) : oper_(oper) {}
	oper_type getValue() { return oper_; }

	Token::pointer_type accept(ExprVisitor &v) override final;
};

class LogicalExpression : public Expression
{
public:
	DEF_POINTER_TYPE(LogicalExpression)
		DEF_IS_CONVERTIBLE_FROM(LogicalExpression)
		using oper_type = std::shared_ptr<BinaryOperator>;
public:
	expr_type left_;
	oper_type oper_;
	expr_type right_;
public:
	LogicalExpression() = delete;
	LogicalExpression(expr_type left, oper_type oper, expr_type right) : left_(left), oper_(oper), right_(right) {}
	oper_type getValue() { return oper_; }

	Token::pointer_type accept(ExprVisitor &v) override final;
};

class VariableExpression : public Expression
{
public:
	DEF_POINTER_TYPE(VariableExpression)
	DEF_IS_CONVERTIBLE_FROM(VariableExpression)
	using var_type = Variable::pointer_type;
public:
	var_type oper_;
public:
	VariableExpression() = delete;
	VariableExpression(var_type oper) : oper_(oper) {}
	var_type getValue() { return oper_; }

	Token::pointer_type accept(ExprVisitor &v) override final;
};