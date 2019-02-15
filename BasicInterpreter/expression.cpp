#include "expression.hpp"
#include "visitor.hpp"

/**
@name:		accept
@purpose:	passes a pointer to itself to the Visitor's visit() method
@param:		ExprVisitor & 
@return:	Token::pointer_type
*/
Token::pointer_type BinaryExpression::accept(ExprVisitor & v)
{
	return v.visit(this);
}

/**
@name:		accept
@purpose:	passes a pointer to itself to the Visitor's visit() method
@param:		ExprVisitor &
@return:	Token::pointer_type
*/
Token::pointer_type UnaryExpression::accept(ExprVisitor & v)
{
	return v.visit(this);
}

/**
@name:		accept
@purpose:	passes a pointer to itself to the Visitor's visit() method
@param:		ExprVisitor &
@return:	Token::pointer_type
*/
Token::pointer_type GroupExpression::accept(ExprVisitor & v)
{
	return v.visit(this);
}

/**
@name:		accept
@purpose:	passes a pointer to itself to the Visitor's visit() method
@param:		ExprVisitor &
@return:	Token::pointer_type
*/
Token::pointer_type LiteralExpression::accept(ExprVisitor & v)
{
	return v.visit(this);
}

/**
@name:		accept
@purpose:	passes a pointer to itself to the Visitor's visit() method
@param:		ExprVisitor &
@return:	Token::pointer_type
*/
Token::pointer_type VariableExpression::accept(ExprVisitor & v)
{
	return v.visit(this);
}

/**
@name:		accept
@purpose:	passes a pointer to itself to the Visitor's visit() method
@param:		ExprVisitor &
@return:	Token::pointer_type
*/
Token::pointer_type AssignExpression::accept(ExprVisitor & v)
{
	return v.visit(this);
}

/**
@name:		accept
@purpose:	passes a pointer to itself to the Visitor's visit() method
@param:		ExprVisitor &
@return:	Token::pointer_type
*/
Token::pointer_type LogicalExpression::accept(ExprVisitor & v)
{
	return v.visit(this);
}

/**
@name:		accept
@purpose:	passes a pointer to itself to the Visitor's visit() method
@param:		ExprVisitor &
@return:	Token::pointer_type
*/
Token::pointer_type CallExpression::accept(ExprVisitor & v)
{
	return v.visit(this);
}

/**
@name:		accept
@purpose:	passes a pointer to itself to the Visitor's visit() method
@param:		ExprVisitor &
@return:	Token::pointer_type
*/
Token::pointer_type GetExpression::accept(ExprVisitor & v)
{
	return v.visit(this);
}

/**
@name:		accept
@purpose:	passes a pointer to itself to the Visitor's visit() method
@param:		ExprVisitor &
@return:	Token::pointer_type
*/
Token::pointer_type SetExpression::accept(ExprVisitor & v)
{
	return v.visit(this);
}

/**
@name:		accept
@purpose:	passes a pointer to itself to the Visitor's visit() method
@param:		ExprVisitor &
@return:	Token::pointer_type
*/
Token::pointer_type MeExpression::accept(ExprVisitor & v)
{
	return v.visit(this);
}

/**
@name:		accept
@purpose:	passes a pointer to itself to the Visitor's visit() method
@param:		ExprVisitor &
@return:	Token::pointer_type
*/
Token::pointer_type SuperExpression::accept(ExprVisitor & v)
{
	return v.visit(this);
}
