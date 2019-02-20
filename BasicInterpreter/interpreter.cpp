/**	@file interpreter.cpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of Intepreter class
*/

#include "interpreter.hpp"
#include "integer.hpp"
#include "float.hpp"
#include "boolean.hpp"
#include "word.hpp"
#include "variable.hpp"
#include "beta_callable.hpp"
#include "math_func.hpp"
#include "misc_func.hpp"
#include "custom_func.hpp"
#include "return.hpp"
#include "beta_instance.hpp"
#include "beta_class.hpp"
#include <stack>
#include <iostream>
#include <sstream>
using namespace std;

Interpreter::Interpreter()
{
	globals_.reset(new Environment());
	prepareNativeFunctions();
	env_ = globals_;
}

/**
@name:		VarTypeName
@purpose:	returns the variable type as a string
@param:		Variable::VarType
@return:	std::string
*/
std::string Interpreter::VarTypeName(Variable::VarType type)
{
	if (type == Variable::VarType::BOOL)
		return "bool";
	else if (type == Variable::VarType::INT)
		return "int";
	else if (type == Variable::VarType::FLOAT)
		return "float";
	else if (type == Variable::VarType::WORD)
		return "word";
	else
		return "object";

}

/**
@name:		checkType
@purpose:	ensures the variable and the value being assigned to it are the same type
@param:		Variable::VarType, Token::pointer_type
@return:	bool
*/
bool Interpreter::checkType(Variable::VarType type, Token::pointer_type val)
{
	if ((type == Variable::VarType::BOOL && is<Boolean>(val)) ||
		(type == Variable::VarType::INT && is<Integer>(val)) ||
		(type == Variable::VarType::FLOAT && is<Float>(val)) ||
		(type == Variable::VarType::WORD && is<Word>(val)) ||
		type == Variable::VarType::OBJECT && is<BetaInstance>(val))
		return true;

	return false;
}

/**
@name:		visit
@purpose:	reassigns a variable in an Assignment Expression
@param:		AssignExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Interpreter::visit(AssignExpression * expr)
{
	tok_type val = evaluate(expr->expr_);
	Variable::VarType type = expr->oper_->getType();

	if (!checkType(type, val))
		throw InterpreterException(string("InterpreterException: Cannot assign \"" + val->toString() + "\" to a " + VarTypeName(type)).c_str());

	//env_->reassign(expr->oper_->getName(), val);
	auto itVar = locals_.find(expr);
	if (itVar != locals_.end())
		env_->reassignAt(itVar->second, expr->oper_->getName(), val);
	else
		globals_->reassign(expr->oper_->getName(), val);

	return val;
}

/**
@name:		visit
@purpose:	Evaluates a Binary Expression and its operands
@param:		BinaryExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Interpreter::visit(BinaryExpression * expr)
{
	tok_type left = evaluate(expr->left_);
	tok_type right = evaluate(expr->right_);
	
	if (!is<BinaryOperator>(expr->oper_))
	{
		stringstream ex;
		ex << "The operator <" + expr->oper_->toString() + "> is not a binary operator.";
		throw exception(ex.str().c_str());
	}

	stack<oper_type> operStack;
	operStack.push(convert<Operand>(right));
	operStack.push(convert<Operand>(left));
	return convert<Token>(expr->oper_->perform(operStack));
}

/**
@name:		visit
@purpose:	Evaluates and calls a function in a Call Expression
@param:		CallExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Interpreter::visit(CallExpression * expr)
{
	tok_type callee = evaluate(expr->callee_);

	if (callee == nullptr)
	{
		stringstream ss;
		ss << "\"" << callee->toString() << "\" is not initialized!";
		throw InterpreterException(ss.str().c_str());
	}
		
	if (!is<BetaCallable>(callee))
		throw InterpreterException("Can only call functions.");
	
	list<tok_type> args;
	for (auto arg : expr->args_)
		args.push_back(evaluate(arg));

	BetaCallable::pointer_type func = convert<BetaCallable>(callee);
	if (args.size() != func->arity())
	{
		stringstream ss;
		ss << "Expected " << func->arity() << " arguments, but received " << args.size() << ".";
		throw InterpreterException(ss.str().c_str());
	}

	return func->call(this, args);
}

/**
@name:		visit
@purpose:	Evaluates and returns a Unary Expression
@param:		UnaryExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Interpreter::visit(UnaryExpression * expr)
{
	tok_type right = evaluate(expr->exp_);
	if (!is<UnaryOperator>(expr->oper_))
	{
		stringstream ex;
		ex << "The operator <" + expr->oper_->toString() + "> is not a unary operator.";
		throw exception(ex.str().c_str());
	}

	stack<oper_type> operStack;
	operStack.push(convert<Operand>(right));
	return convert<Token>(expr->oper_->perform(operStack));
}

/**
@name:		visit
@purpose:	Evaluates and returns a Group Expression
@param:		GroupExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Interpreter::visit(GroupExpression * expr)
{
	return evaluate(expr->expr_);
}

/**
@name:		visit
@purpose:	Evaluates and returns a Literal Expression
@param:		LiteralExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Interpreter::visit(LiteralExpression * expr)
{
	return convert<Token>(expr->getValue());
}

/**
@name:		visit
@purpose:	returns a variable stored in an environment
@param:		VariableExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Interpreter::visit(VariableExpression * expr)
{
	return lookUpVariable(expr->oper_->getName(), expr);
}

/**
@name:		visit
@purpose:	Evaluates and returns the result of a logical expression
@param:		LogicalExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Interpreter::visit(LogicalExpression * expr)
{
	tok_type left = evaluate(expr->left_);

	if (is<Boolean>(left))
	{
		Boolean::pointer_type cond = convert<Boolean>(left);
		if (is<Or>(expr->oper_))
			if (cond->getValue() == true)	// an Or statement in which the first statement is true
				return left;
		else if (is<And>(expr->oper_))
			if (cond->getValue() == false)	// an And statement in which the first statement is false
				return left;
		else if (!is<Or>(expr->oper_) || !is<And>(expr->oper_))
			throw InterpreterException("Condition must evaluate to true or false.");

		return evaluate(expr->right_);
	}

	throw InterpreterException("Condition must evaluate to true or false.");
}

/**
@name:		visit
@purpose:	Returns a variable stored in a Beta object
@param:		GetExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Interpreter::visit(GetExpression * expr)
{
	tok_type obj = evaluate(expr->obj_);

	if (obj == nullptr)
	{
		stringstream ss;
		ss << "\"" << obj->toString() << "\" is not initialized!";
		throw InterpreterException(ss.str().c_str());
	}

	if (is<BetaInstance>(obj))
		return convert<BetaInstance>(obj)->get(expr->name_->getName());

	stringstream ss;
	ss << "InterpreterException: " << "\"" << obj->toString() << "\" is not an object.";
	throw InterpreterException(ss.str().c_str());
}

/**
@name:		visit
@purpose:	Sets a variable stored in a Beta object
@param:		SetExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Interpreter::visit(SetExpression * expr)
{
	tok_type obj = evaluate(expr->obj_);

	if (!is<BetaInstance>(obj))
	{
		stringstream ss;
		ss << "InterpreterException: " << "\"" << obj->toString() << "\" is not an object.";
		throw InterpreterException(ss.str().c_str());
	}

	Token::pointer_type val = evaluate(expr->val_);
	convert<BetaInstance>(obj)->set(expr->name_->getName(), val);
	return val;
		
}

/**
@name:		visit
@purpose:	Returns a pointer to a BetaClass instance
@param:		MeExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Interpreter::visit(MeExpression * expr)
{
	return lookUpVariable(expr->keyword_->getName(), expr);
}

/**
@name:		visit
@purpose:	Returns a class method found in a parent class
@param:		SuperExpression *
@return:	Token::pointer_type
*/
Token::pointer_type Interpreter::visit(SuperExpression * expr)
{
	unsigned dist = locals_.find(expr)->second;	// find the correct scope
	BetaClass::pointer_type super = convert<BetaClass>(env_->getAt(dist, "super"));

	// the class instance is always one scope below super
	BetaInstance::pointer_type obj = convert<BetaInstance>(env_->getAt(dist - 1, "me"));
	// bind "me" to the superclass's method and then return it
	CustomFunction::pointer_type method = super->findMethod(obj, expr->method_->getName());

	if (method == nullptr)
	{
		stringstream ss;
		ss << "InterpreterException: \"" << expr->method_->getName() << "\" is undefined.";
		throw InterpreterException(ss.str().c_str());
	}

	return tok_type(method);
}

/**
@name:		visit
@purpose:	Evaluates a generic statement
@param:		StmtExpression *
@return:	void
*/
void Interpreter::visit(StmtExpression * expr)
{
	evaluate(expr->expr_);
}

/**
@name:		visit
@purpose:	Evaluates and prints the result of a statement
@param:		StmtPrint *
@return:	void
*/
void Interpreter::visit(StmtPrint * expr)
{
	tok_type val = evaluate(expr->expr_);
	cout << val->toString() << endl;
}

/**
@name:		visit
@purpose:	Evaluates a Return Statement and throws a Return Exception
@param:		StmtReturn *
@return:	void
*/
void Interpreter::visit(StmtReturn * expr)
{
	tok_type val = nullptr;
	if (expr->expr_ != nullptr) val = evaluate(expr->expr_);

	throw ReturnEx(val);
}

/**
@name:		visit
@purpose:	Evaluates and stores the value of an expression
@param:		StmtVariable *
@return:	void
*/
void Interpreter::visit(StmtVariable * expr)
{
	tok_type tok;
	if (expr->expr_ != nullptr)
	{
		tok = evaluate(expr->expr_);
		Variable::VarType type = expr->var_->getType();

		if (!checkType(type, tok))
			throw InterpreterException(string("InterpreterException: Cannot assign \"" + tok->toString() + "\" to a " + VarTypeName(type)).c_str());
	}
		
	env_->define(expr->var_->getName(), tok);
}

/**
@name:		visit
@purpose:	Executes a block of statements
@param:		StmtBlock *
@return:	void
*/
void Interpreter::visit(StmtBlock * expr)
{
	executeBlock(expr->statements_, env_);
}

/**
@name:		visit
@purpose:	Creates and stores a Beta Object in memory
@param:		StmtClass *
@return:	void
*/
void Interpreter::visit(StmtClass * expr)
{
	tok_type super = nullptr;
	if (expr->super_ != nullptr)
	{
		super = evaluate(expr->super_);
		if (!is<BetaClass>(super))
		{
			stringstream ss;
			ss << "InterpreterException: \"" << super->toString() << "\" is not a known class.";
			throw InterpreterException(ss.str().c_str());
		}
	}

	// defines the uninitialized class in memory
	env_->define(expr->name_->getName(), nullptr);

	// super is just inside of the class's environment
	if (expr->super_ != nullptr)
	{
		env_.reset(new Environment(env_));
		env_->define("super", super);
	}

	// stores each method, and whether or not it is the init() method
	map<string, CustomFunction::pointer_type> methods;
	list<Variable::pointer_type> data;

	for (auto datum : expr->data_)
		data.push_back(datum);

	for (auto method : expr->methods_)
		methods.insert(pair<string, CustomFunction::pointer_type>(method->ident_->getName(), CustomFunction::pointer_type(new CustomFunction(method, env_, method->ident_->getName() == "init"))));

	BetaClass::pointer_type cla(new BetaClass(expr->name_->getName(), convert<BetaClass>(super), data, methods));

	// reset the environment if there is no super class (all methods are below the super instance)
	if (super != nullptr)
		env_ = env_->parent_;

	env_->reassign(expr->name_->getName(), cla);
}

/**
@name:		visit
@purpose:	Evaluates an if-statement
@param:		StmtIf *
@return:	void
*/
void Interpreter::visit(StmtIf * expr)
{
	tok_type eval = evaluate(expr->expr_);
	if (is<Boolean>(eval))
	{
		Boolean::pointer_type cond = convert<Boolean>(eval);
		if (cond->getValue() == true)
			execute(expr->thenBlock_);
		
		// don't evaluate if there is no else block
		if (expr->elseBlock_ != nullptr)
			if (cond->getValue() == false)
				execute(expr->elseBlock_);

		return;
	}

	throw InterpreterException("InterpreterException: If-Statement condition must evaluate to true or false.");
}

/**
@name:		visit
@purpose:	Evaluates a while-statement
@param:		StmtWhile *
@return:	void
*/
void Interpreter::visit(StmtWhile * expr)
{
	tok_type cond = evaluate(expr->expr_);
	while (is<Boolean>(cond) && convert<Boolean>(cond)->getValue() == true)
	{
		execute(expr->whileBlock_);
		cond = evaluate(expr->expr_);
	}

	if (!is<Boolean>(cond))
		throw InterpreterException("While-loop condition must evaluate to true or false.");
}

/**
@name:		visit
@purpose:	Evaluates a do while-statement
@param:		StmtDoWhile *
@return:	void
*/
void Interpreter::visit(StmtDoWhile * expr)
{
	tok_type cond;
	do
	{
		execute(expr->whileBlock_);
		cond = evaluate(expr->expr_);
	} 
	while (is<Boolean>(cond) && convert<Boolean>(cond)->getValue() == true);

	if (!is<Boolean>(cond))
		throw InterpreterException("Do-While loop condition must evaluate to true or false.");
}

/**
@name:		visit
@purpose:	Evaluates a function statement
@param:		StmtFunc *
@return:	void
*/
void Interpreter::visit(StmtFunc * expr)
{
	CustomFunction::pointer_type func(new CustomFunction(shared_ptr<StmtFunc>(expr), env_, false));
	env_->define(expr->ident_->toString(), tok_type(func));
}

/**
@name:		prepareNativeFunctions
@purpose:	Creates a series of global native functions
@param:		void
@return:	void
*/
void Interpreter::prepareNativeFunctions()
{
	globals_->define("abs", make<Abs>());
	globals_->define("mod", make<Modulus>());
	globals_->define("cos", make<Cos>());
	globals_->define("exp", make<Expo>());
	globals_->define("log", make<Log>());
	globals_->define("sin", make<Sin>());
	globals_->define("sqr", make<Sqr>());
	globals_->define("tan", make<Tan>());
	globals_->define("datetime", make<DateTime>());
	globals_->define("datetimeUTC", make<DateTimeUTC>());
	globals_->define("epoch", make<Epoch>());
}

/**
@name:		evaluate
@purpose:	Calls the Expression's accept method
@param:		Interpreter::expr_type
@return:	Interpreter::tok_type
*/
Interpreter::tok_type Interpreter::evaluate(Interpreter::expr_type expr)
{
	return expr->accept(*this);
}

/**
@name:		execute
@purpose:	Calls the Statement's accept method
@param:		Interpreter::stmt_p
@return:	void
*/
void Interpreter::execute(stmt_p stmt)
{
	stmt->accept(*this);
}

/**
@name:		lookUpVariable
@purpose:	Finds a variable in a higher scope, or in the global scope
@param:		std::string, Expression *
@return:	Token::pointer_type
*/
Token::pointer_type Interpreter::lookUpVariable(std::string name, Expression * expr)
{
	auto itVar = locals_.find(expr);
	if (itVar != locals_.end())
		return env_->getAt(itVar->second, name);
	
	return globals_->get(name);
}

/**
@name:		interpret
@purpose:	Evaluates every statement in the programme
@param:		Interpreter::stmt_list
@return:	void
*/
void Interpreter::interpret(stmt_list lstState)
{
	for (auto state : lstState)
		execute(state);
}

/**
@name:		executeBlock
@purpose:	Exectutes a block of statements, std::shared_ptr<Environment>
@param:		Interpreter::stmt_list
@return:	void
*/
void Interpreter::executeBlock(stmt_list statements, std::shared_ptr<Environment> env)
{
	// reset the environment, as this may be a function or a method
	std::shared_ptr<Environment> prev = env_;
	env_ = env;

	for (auto state : statements)
		execute(state);

	env_ = prev;
}

/**
@name:		resolve
@purpose:	Inserts an expression tied to a variable, and how many scopes the variable is above said expression
@param:		Expression *, unsigned
@return:	void
*/
void Interpreter::resolve(Expression * expr, unsigned depth)
{
	locals_.insert(pair<Expression*, unsigned>(expr, depth));
}
