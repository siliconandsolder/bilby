/**	@file resolver.cpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of Resolver class
*/

#include "resolver.hpp"
#include "position_tracker.hpp"
#include <sstream>
using namespace std;

/**
@name:		visit
@purpose:	checks a variable statement's expression for any variable references
@param:		StmtVariable *
@return:	void
*/
void Resolver::visit(StmtVariable * expr)
{
	declare(expr->var_);
	if (expr->expr_ != nullptr)
		resolve(expr->expr_);
	define(expr->var_);
}

/**
@name:		visit
@purpose:	checks a block statement's expression for any variable references
@param:		StmtBlock *
@return:	void
*/
void Resolver::visit(StmtBlock * expr)
{
	// create new scope that encompasses block
	beginScope();
	resolve(expr->statements_);
	endScope();
}

/**
@name:		visit
@purpose:	declares and defines a class statements class, methods, and properties
@param:		StmtClass *
@return:	void
*/
void Resolver::visit(StmtClass * expr)
{
	curClass_ = ClassType::CLASS;

	declare(expr->name_);
	
	if (expr->super_ != nullptr)
	{
		curClass_ = ClassType::SUBCLASS;
		resolve(expr->super_);
	}

	define(expr->name_);

	// add the super class to scope if there is one
	if (expr->super_ != nullptr)
	{
		beginScope();
		scopes_.back().insert(pair<string, bool>("super", true));
	}

	// method and variable scope
	beginScope();
	for (auto datum : expr->data_)
	{
		declare(datum);
		define(datum);
	}

	scopes_.back().insert(pair<string, bool>("me", true));
	
	for (auto method : expr->methods_)
	{
		if (method->ident_->getName() == "init")
			resolveFunction(&*method, FuncType::INIT);
		else
			resolveFunction(&*method, FuncType::METHOD);
	}
		

	endScope();

	if (expr->super_ != nullptr)
		endScope();

	curClass_ = ClassType::NONE;
}

/**
@name:		visit
@purpose:	resolves an if-statement's condition, block statment, and else statement
@param:		StmtIf *
@return:	void
*/
void Resolver::visit(StmtIf * expr)
{
	resolve(expr->expr_);
	resolve(expr->thenBlock_);
	if (expr->elseBlock_) resolve(expr->elseBlock_);
}

/**
@name:		visit
@purpose:	resolves a while statement's condition and block statement
@param:		StmtWhile *
@return:	void
*/
void Resolver::visit(StmtWhile * expr)
{
	resolve(expr->expr_);
	resolve(expr->whileBlock_);
}

/**
@name:		visit
@purpose:	resolves a do while-statement's condition and block statement
@param:		StmtWhile *
@return:	void
*/
void Resolver::visit(StmtDoWhile * expr)
{
	resolve(expr->expr_);
	resolve(expr->whileBlock_);
}

/**
@name:		resolve
@purpose:	resolves a list of statements
@param:		std::list<Statement::pointer_type>
@return:	void
*/
void Resolver::resolve(std::list<Statement::pointer_type> statements)
{
	for (auto statement : statements)
	{
		resolve(statement);
		++PositionTracker::itStmtPos_;
	}
		
}

Resolver::Resolver(Interpreter * interpreter)
{
	interpreter_ = interpreter;
	curFunc_ = FuncType::NONE;
	curClass_ = ClassType::NONE;
	
	// superglobal scope
	beginScope();
	
	for (auto global : interpreter->globals_->variables_)
		scopes_.back().insert(pair<string, bool>(global.first, global.second));
}

/**
@name:		resolve
@purpose:	resolves a single statement
@param:		Statement::pointer_type
@return:	void
*/
void Resolver::resolve(Statement::pointer_type statement)
{
	statement->accept(*this);
}

/**
@name:		beginScope
@purpose:	begins a new scope
@param:		null
@return:	void
*/
void Resolver::beginScope()
{
	scopes_.push_back(map<string, bool>());
}

/**
@name:		endScope
@purpose:	removes a scope
@param:		null
@return:	void
*/
void Resolver::endScope()
{
	scopes_.pop_back();
}

/**
@name:		resolve
@purpose:	resolves an expression
@param:		std::shared_ptr<Expression>
@return:	void
*/
void Resolver::resolve(std::shared_ptr<Expression> expr)
{
	expr->accept(*this);
}

/**
@name:		declare
@purpose:	adds a variable to scope that has not yet been defined
@param:		std::string
@return:	void
*/
void Resolver::declare(Token::pointer_type token)
{
	if (scopes_.empty()) return;

	if (scopes_.back().count(token->toString()) == 1)
	{
		// PositionTracker::lastTok_ = token;
		stringstream ss;
		ss << "ResolverException: Variable \"" << token->toString() << "\" already declared in this scope.";
		throw ResolverException(ss.str());
	}
		
	scopes_.back().insert(pair<string, bool>(token->toString(), false));
}

/**
@name:		define
@purpose:	confirms that a variable definition has been found somewhere in the code
@param:		std::string
@return:	void
*/
void Resolver::define(Token::pointer_type token)
{
	if (scopes_.empty()) return;

	scopes_.back().at(token->toString()) = true;
}

/**
@name:		resolveLocal
@purpose:	inserts a reference to a declared variable into the interpreter's memory, as well as the relative scope in which 
			the declaration was made.
@param:		Expression *, std::string
@return:	void
*/
void Resolver::resolveLocal(Expression * expr, Token::pointer_type token)
{
	for (int i = scopes_.size() - 1; i >= 0; --i)
	{
		if (scopes_.at(i).count(token->toString()) == 1)
		{
			interpreter_->resolve(expr, scopes_.size() - 1 - i);
			return;
		}
	}

	/*PositionTracker::lastExp_ = Expression::pointer_type(expr);
	PositionTracker::lastTok_ = token;*/
	stringstream ss;
	ss << "ResolverException: Variable \"" << token->toString() << "\" does not exist anywhere in memory.";
	throw ResolverException(ss.str());

}

/**
@name:		resolveFunction
@purpose:	resolves the parameters and the body of a function
@param:		StmtFunc *, FuncType
@return:	void
*/
void Resolver::resolveFunction(StmtFunc * func, FuncType type)
{
	FuncType parType = curFunc_;
	curFunc_ = type;

	beginScope();
	for (auto param : func->params_)
	{
		declare(param);
		define(param);
	}
	resolve(func->body_);
	endScope();

	curFunc_ = parType;
}

/**
@name:		visit
@purpose:	resolves the expression in a variable assignment
@param:		AssignExpression *
@return:	Token::pointer_type 
*/
Token::pointer_type Resolver::visit(AssignExpression * expr)
{
	resolve(expr->expr_);
	resolveLocal(expr, expr->oper_);
	return nullptr;
}

/**
@name:		visit
@purpose:	resolves both expressions in binary expression
@param:		BinaryExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Resolver::visit(BinaryExpression * expr)
{
	resolve(expr->left_);
	resolve(expr->right_);
	return nullptr;
}

/**
@name:		visit
@purpose:	resolves the arguments and method of a call expression
@param:		CallExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Resolver::visit(CallExpression * expr)
{
	resolve(expr->callee_);
	for(auto arg : expr->args_)
		resolve(arg);

	return nullptr;
}

/**
@name:		visit
@purpose:	resolves the expression of a unary expression
@param:		UnaryExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Resolver::visit(UnaryExpression * expr)
{
	resolve(expr->exp_);
	return nullptr;
}

/**
@name:		visit
@purpose:	resolves the expression of a group expression
@param:		GroupExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Resolver::visit(GroupExpression * expr)
{
	resolve(expr->expr_);
	return nullptr;
}

/**
@name:		visit
@purpose:	resolves the expression of a literal expression
@param:		LiteralExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Resolver::visit(LiteralExpression * expr)
{
	return nullptr;
}

/**
@name:		visit
@purpose:	resolves the left and right expressions of a logical expression
@param:		LogicalExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Resolver::visit(LogicalExpression * expr)
{
	resolve(expr->left_);
	resolve(expr->right_);
	return nullptr;
}

/**
@name:		visit
@purpose:	ensures that the variable has been declared, and inserts a reference to it's expression into the interpreter 
@param:		VariableExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Resolver::visit(VariableExpression * expr)
{
	if (!scopes_.empty() && !(scopes_.back().at(expr->oper_->getName())))
	{
		/*PositionTracker::lastExp_ = Expression::pointer_type(expr);
		PositionTracker::lastTok_ = expr->oper_;*/
		stringstream ss;
		ss << "ResolverException: " << "Cannot read local variable \"" << expr->oper_->getName() << "\" in its own initializer!";
		throw ResolverException(ss.str());
	}

	resolveLocal(expr, expr->oper_);
	return nullptr;
}

/**
@name:		visit
@purpose:	resolves the object of a get expression
@param:		GetExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Resolver::visit(GetExpression * expr)
{
	resolve(expr->obj_);
	return nullptr;
}

/**
@name:		visit
@purpose:	resolves the object and value of a set expression
@param:		SetExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Resolver::visit(SetExpression * expr)
{
	resolve(expr->val_);
	resolve(expr->obj_);
	return nullptr;
}

/**
@name:		visit
@purpose:	inserts a reference to the "me" keyword into the interpreter
@param:		MeExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Resolver::visit(MeExpression * expr)
{
	if (curClass_ != ClassType::CLASS)
	{
		//PositionTracker::lastTok_ = expr->keyword_;
		//PositionTracker::lastExp_ = Expression::pointer_type(expr);
		throw exception("ResolverException: Cannot use 'me' keyword outside of a class.");
	}
		
	resolveLocal(expr, expr->keyword_);
	return nullptr;
}

/**
@name:		visit
@purpose:	inserts a reference to the "super" keyword into the interpreter
@param:		SuperExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Resolver::visit(SuperExpression * expr)
{
	if (curClass_ == ClassType::NONE)
	{
		/*PositionTracker::lastExp_ = Expression::pointer_type(expr);
		PositionTracker::lastTok_ = expr->keyword_;*/
		throw exception("Cannot use 'super' keyword outside of class.");
	}
	else if (curClass_ != ClassType::SUBCLASS)
	{
		/*PositionTracker::lastExp_ = Expression::pointer_type(expr);
		PositionTracker::lastTok_ = expr->keyword_;*/
		throw exception("Cannot use 'super' keyword in class with no superclass.");
	}

	resolveLocal(expr, expr->keyword_);
	return nullptr;
}

/**
@name:		visit
@purpose:	resolves the expression of a generic expression statement
@param:		StmtExpression *
@return:	void
*/
void Resolver::visit(StmtExpression * expr)
{
	resolve(expr->expr_);
}

/**
@name:		visit
@purpose:	resolves all variables within a function statement
@param:		StmtFunc *
@return:	void
*/
void Resolver::visit(StmtFunc * expr)
{
	declare(expr->ident_);
	define(expr->ident_);
	resolveFunction(expr, FuncType::FUNC);
}

/**
@name:		visit
@purpose:	resolves the expression in a print statement
@param:		StmtPrint *
@return:	void
*/
void Resolver::visit(StmtPrint * expr)
{
	resolve(expr->expr_);
}

/**
@name:		visit
@purpose:	resolves the expression within a return statement
@param:		StmtReturn *
@return:	void
*/
void Resolver::visit(StmtReturn * expr)
{
	if (curFunc_ == FuncType::NONE)
	{
		/*PositionTracker::lastExp_ = Expression::pointer_type(expr->expr_);
		PositionTracker::lastTok_ = expr->ret_;*/
		throw ResolverException("ResolverException: Cannot return from outside of a function!");
	}

	if (expr->expr_)
	{
		if (curFunc_ == FuncType::INIT)
		{
			/*PositionTracker::lastExp_ = Expression::pointer_type(expr->expr_);
			PositionTracker::lastTok_ = expr->ret_;*/
			throw ResolverException("ResolverException: Cannot return from an init function!");
		}

		resolve(expr->expr_);
	}
		
}
