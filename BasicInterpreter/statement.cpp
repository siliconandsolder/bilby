#include "statement.hpp"
#include "visitor.hpp"

/**
@name:		visit
@purpose:	passes a pointer to an expression statement to a statement visitor
@param:		StmtVisitor &
@return:	void
*/
void StmtExpression::accept(StmtVisitor & v)
{
	v.visit(this);
}

/**
@name:		visit
@purpose:	passes a pointer to a print statement to a statement visitor
@param:		StmtVisitor &
@return:	void
*/
void StmtPrint::accept(StmtVisitor & v)
{
	v.visit(this);
}

/**
@name:		visit
@purpose:	passes a pointer to a variable statement to a statement visitor
@param:		StmtVisitor &
@return:	void
*/
void StmtVariable::accept(StmtVisitor & v)
{
	v.visit(this);
}

/**
@name:		visit
@purpose:	passes a pointer to a block statement to a statement visitor
@param:		StmtVisitor &
@return:	void
*/
void StmtBlock::accept(StmtVisitor & v)
{
	v.visit(this);
}

/**
@name:		visit
@purpose:	passes a pointer to an if statement to a statement visitor
@param:		StmtVisitor &
@return:	void
*/
void StmtIf::accept(StmtVisitor & v)
{
	v.visit(this);
}

/**
@name:		visit
@purpose:	passes a pointer to a while statement to a statement visitor
@param:		StmtVisitor &
@return:	void
*/
void StmtWhile::accept(StmtVisitor & v)
{
	v.visit(this);
}

/**
@name:		visit
@purpose:	passes a pointer to a do while statement to a statement visitor
@param:		StmtVisitor &
@return:	void
*/
void StmtDoWhile::accept(StmtVisitor & v)
{
	v.visit(this);
}

/**
@name:		visit
@purpose:	passes a pointer to a function statement to a statement visitor
@param:		StmtVisitor &
@return:	void
*/
void StmtFunc::accept(StmtVisitor & v)
{
	v.visit(this);
}

/**
@name:		visit
@purpose:	passes a pointer to a return statement to a statement visitor
@param:		StmtVisitor &
@return:	void
*/
void StmtReturn::accept(StmtVisitor & v)
{
	v.visit(this);
}

/**
@name:		visit
@purpose:	passes a pointer to a class statement to a statement visitor
@param:		StmtVisitor &
@return:	void
*/
void StmtClass::accept(StmtVisitor & v)
{
	v.visit(this);
}
