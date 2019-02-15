#include "ast_printer.hpp"
#include "integer.hpp"
#include "float.hpp"
#include "boolean.hpp"
#include "word.hpp"
#include "variable.hpp"
#include <vector>

using namespace std;

std::string AstPrinter::print(Expression::expr_type exp)
{
	exp->accept(*this);
	return ss_.str();
}

Token::pointer_type AstPrinter::visit(BinaryExpression * expr)
{
	parenthesize(expr->oper_->toString(), vector<Expression::expr_type>{expr->left_, expr->right_ });
	return Token::pointer_type();
}

Token::pointer_type AstPrinter::visit(UnaryExpression * expr)
{
	parenthesize(expr->oper_->toString(), vector<Expression::expr_type>{expr->exp_});
	return Token::pointer_type();
}

Token::pointer_type AstPrinter::visit(GroupExpression * expr)
{
	parenthesize("group", vector<Expression::expr_type>{expr->expr_});
	return Token::pointer_type();
}

Token::pointer_type AstPrinter::visit(LiteralExpression * expr)
{
	LiteralExpression::oper_type operand = expr->getValue();
	ss_ << operand->toString();
	return Token::pointer_type();
}

Token::pointer_type AstPrinter::visit(VariableExpression * expr)
{
	return Token::pointer_type();
}

void AstPrinter::parenthesize(std::string name, std::vector<Expression::expr_type> exprs)
{
	ss_.clear();
	
	ss_ << "(" << name;
	for (auto exp : exprs)
	{
		ss_ << " ";
		exp->accept(*this);
	}
	ss_ << ")";
}
