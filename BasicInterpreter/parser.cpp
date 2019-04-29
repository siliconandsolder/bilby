/**	@file parser.cpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of Parser class
*/

#include "parser.hpp"
#include "operand.hpp"
#include "operator.hpp"
#include "keywords.hpp"
#include "integer.hpp"
#include "float.hpp"
#include "boolean.hpp"
#include "word.hpp"
#include <stack>
#include <exception>
using namespace std;

/**
@name:		parse
@purpose:	parses a list of statements
@param:		null
@return:	Parser::stmt_list
*/
Parser::stmt_list Parser::parse()
{
	stmt_list lstState;
	while (!isAtEnd())
		lstState.push_back(statement());
	
	return lstState;
}

/**
@name:		statement
@purpose:	returns a statement pointer based on the opening keyword
@param:		null
@return:	Parser::stmt_p
*/
Parser::stmt_p Parser::statement()
{
	if (check<If>()) return ifStatement();
	else if (check<Class>()) return classStatement();
	//else if (check<Func>()) return funcStatement("function");
	else if (check<For>()) return forStatement();
	else if (check<While>()) return whileStatement();
	else if (check<Do>()) return doWhileStatement();
	else if (check<Print>()) return printStatement();
	else if (check<Return>()) return retStatement();
	else if (check<Var>()) return varDeclaration();
	else if (check<LeftBrace>()) return block();

	return expStatement();
}

/**
@name:		classStatement
@purpose:	creates a new class statement
@param:		null
@return:	Parser::stmt_p
*/
Parser::stmt_p Parser::classStatement()
{
	Variable::pointer_type name = convert<Variable>(consume<Variable>("Expected class name."));
	
	VariableExpression::pointer_type super = nullptr;
	if (check<Inherits>())
	{
		consume<Variable>("Expected superclass name.");
		super.reset(new VariableExpression(convert<Variable>(previous())));
	}
	
	consume<LeftBrace>("Expected '{' before class body.");

	list<Variable::pointer_type> data;
	list<StmtFunc::pointer_type> methods;

	if (check<Data>())
	{
		consume<Colon>("Expected colon after \"data\" keyword.");
		// parse all variables
		while (!is<RightBrace>(peek()) && !is<Method>(peek()))
		{
			VarType type = assignVarType(consume<Var>("Class variables must start with an identifier."));
			
			Variable::pointer_type var = convert<Variable>(consume<Variable>("Expected variable name"));
			var->setType(type);
			data.push_back(var);
			
			consume<SemiColon>("Expected semicolon after variable " + data.back()->getName() + " declaration.");
		}

		if (check<Method>())
		{
			consume<Colon>("Expected colon after \"method\" keyword.");
			// parse all methods
			while (!is<RightBrace>(peek()))
			{
				advance();
				methods.push_back(convert<StmtFunc>(varDeclaration()));
			}
		}
	}
	else if (check<Method>())
	{
		consume<Colon>("Expected colon after \"method\" keyword.");
		// parse all methods
		while (!is<RightBrace>(peek()) && !is<Data>(peek()))
		{
			advance();
			methods.push_back(convert<StmtFunc>(varDeclaration()));
		}

		if (check<Data>())
		{
			consume<Colon>("Expected colon after \"data\" keyword.");
			// parse all variables
			while (!is<RightBrace>(peek()))
			{
				VarType type = assignVarType(consume<Var>("Class variables must start with an identifier."));

				Variable::pointer_type var = convert<Variable>(consume<Variable>("Expected variable name"));
				var->setType(type);
				data.push_back(var);

				consume<SemiColon>("Expected semicolon after variable " + data.back()->getName() + " declaration.");
			}
		}
	}
		
	consume<RightBrace>("Expected closing brace after class definition.");

	return stmt_p(new StmtClass(name, super, data, methods));
}

/**
@name:		printStatement
@purpose:	creates a new print statement
@param:		null
@return:	Parser::stmt_p
*/
Parser::stmt_p Parser::printStatement()
{
	expr_p val = expression();
	consume<SemiColon>("Expected ';'");
	stmt_p printer(new StmtPrint(val));
	return printer;
}

/**
@name:		retStatement
@purpose:	creates a new return statement
@param:		null
@return:	Parser::stmt_p
*/
Parser::stmt_p Parser::retStatement()
{
	auto ret = convert<Return>(previous());
	
	expr_p val = nullptr;
	if (!is<SemiColon>(peek()))
		val = expression();

	consume<SemiColon>("Expected ';' after return statement.");
	return stmt_p(new StmtReturn(ret, val));
}

/**
@name:		expStatement
@purpose:	creates a new expression statement (usually just a simple calculation)
@param:		null
@return:	Parser::stmt_p
*/
Parser::stmt_p Parser::expStatement()
{
	expr_p val = expression();
	consume<SemiColon>("Expected ';'");
	stmt_p expr(new StmtExpression(val));
	return expr;
}

/**
@name:		varDeclaration
@purpose:	creates a new variable statement with a name and expression
@param:		null
@return:	Parser::stmt_p
*/
Parser::stmt_p Parser::varDeclaration()
{
	Var::pointer_type type = convert<Var>(previous());

	Variable::pointer_type var = convert<Variable>(consume<Variable>("Expected a name after type declaration."));
	
	// set the variable type
	if (is<VarBool>(type))
		var->setType(VarType::BOOL);
	else if (is<VarInt>(type) || is<VarFloat>(type))
		var->setType(VarType::NUMBER);
	else if (is<VarWord>(type))
		var->setType(VarType::WORD);
	else if (is<VarObject>(type))
		var->setType(VarType::OBJECT);
	else if (is<VarVoid>(type))	// assumed to be a function/method
		var->setType(VarType::VOID_TYPE);
	else
		throw ParserException(string("Variable \"" + var->getName() + "\" has an unknown type."));
		

	// this is a function
	if (is<LeftBracket>(peek()))
		return funcStatement("function", var);
	
	// tried to assign void to a variable
	if (is<VarVoid>(type))
		throw ParserException(string("\"void\" is only available for functions and methods."));

	expr_p init;
	if (check<Assignment>())
		init = expression();

	consume<SemiColon>("Expected ';'");
	stmt_p expr(new StmtVariable(convert<Variable>(var), init));
	return expr;
}

/**
@name:		assignType
@purpose:	Assigns a type to variables
@param:		Token::pointer_type
@return:	Variable::VarType
*/
VarType Parser::assignVarType(Token::pointer_type tok)
{
	if (is<VarNumber>(tok))
		return VarType::NUMBER;
	else if (is<VarBool>(tok))
		return VarType::BOOL;
	else if (is<VarObject>(tok))
		return VarType::OBJECT;
	else if (is<VarWord>(tok))
		return VarType::WORD;
	else
		throw ParserException(string("\"void\" is only available for functions and methods."));
}

/**
@name:		funcStatement
@purpose:	creates a new function statement with a name, parameters, and a block statement
@param:		null
@return:	Parser::stmt_p
*/
Parser::stmt_p Parser::funcStatement(std::string kind, Variable::pointer_type name)
{
	//Variable::pointer_type name = convert<Variable>(consume<Variable>("Expected " + kind + " name"));

	consume<LeftBracket>("Expected '(' after " + kind + " name");
	
	list<Token::pointer_type> params;
	if (!is<RightBracket>(peek()))
	{
		do
		{
			if (params.size() == 8)
				throw ParserException("Cannot have more than 8 paramaters.");

			VarType type = assignVarType(consume<Var>("Parameters must start with an identifier."));
			
			Token::pointer_type param = consume<Variable>("Expected paramater name.");
			convert<Variable>(param)->setType(type);

			params.push_back(param);
		}
		while (check<Comma>());
	}

	consume<RightBracket>("Expected ')' after " + kind + " parameters");
	consume<LeftBrace>("Expected '{' after " + kind + " header");
	
	stmt_list body;

	while (!is<RightBrace>(peek()) && !isAtEnd())
		body.push_back(statement());

	consume<RightBrace>("Expected '}' after " + kind + ".");

	return stmt_p(new StmtFunc(name, params, body));

}

/**
@name:		block
@purpose:	creates a new block statement (a series of statements surrounded by braces)
@param:		null
@return:	Parser::stmt_p
*/
Parser::stmt_p Parser::block()
{
	stmt_list statements;

	while (!isAtEnd() && !is<RightBrace>(peek()))
		statements.push_back(statement());

	consume<RightBrace>("Expected '}'");
	return stmt_p(new StmtBlock(statements));
}

/**
@name:		ifStatement
@purpose:	creates a new if statement with a condition and option else statement
@param:		null
@return:	Parser::stmt_p
*/
Parser::stmt_p Parser::ifStatement()
{
	consume<LeftBracket>("Expected '(' after 'if'");
	expr_p cond = expression();
	consume<RightBracket>("Expected ')' after if condition.");

	stmt_p thenB = statement();
	stmt_p elseB = nullptr;
	
	if (check<Else>())
		elseB = statement();
		
	return stmt_p(new StmtIf(cond, thenB, elseB));
}

/**
@name:		whileStatement
@purpose:	creates a new while statement with a condition
@param:		null
@return:	Parser::stmt_p
*/
Parser::stmt_p Parser::whileStatement()
{
	consume<LeftBracket>("Expected '(' after 'while'");
	expr_p cond = expression();
	consume<RightBracket>("Expected ')' after while condition.");

	stmt_p whileB = statement();
	return stmt_p(new StmtWhile(cond, whileB));
}

/**
@name:		doWhileStatement
@purpose:	creates a new do while-statement with a condition expression
@param:		null
@return:	Parser::stmt_p
*/
Parser::stmt_p Parser::doWhileStatement()
{
	stmt_p whileB = statement();

	consume<While>("Expected 'while' keyword after 'do' statement.");
	consume<LeftBracket>("Expected '(' after 'while'");
	expr_p cond = expression();
	consume<RightBracket>("Expected ')' after while condition.");

	return stmt_p(new StmtDoWhile(cond, whileB));
}

/**
@name:		forStatement
@purpose:	creates a new for-statement with optional initializer, conditional, and incrementer/decrementer
@param:		null
@return:	Parser::stmt_p
*/
Parser::stmt_p Parser::forStatement()
{
	consume<LeftBracket>("Expected '(' after 'for'");
	
	stmt_p init;
	if (is<SemiColon>(peek()))
		init = nullptr;
	else if (is<Var>(peek()))
		init = varDeclaration();
	else
		init = expStatement();

	expr_p cond = nullptr;
	if (!is<SemiColon>(peek()))
		cond = expression();

	consume<SemiColon>("Expected ';' after for-loop conditional.");

	expr_p inc = nullptr;
	if (!is<RightBracket>(peek()))
		inc = expression();

	consume<RightBracket>("Expected ')' after for-loop declaration.");

	stmt_p bod = statement();

	if (inc != nullptr)
		bod.reset(new StmtBlock(stmt_list{bod, stmt_p(new StmtExpression(inc))}));

	if (cond == nullptr)
		cond.reset(new LiteralExpression(Boolean::pointer_type(new Boolean(true))));

	bod.reset(new StmtWhile(cond, bod));

	if (init != nullptr)
		bod.reset(new StmtBlock(stmt_list{init, bod}));

	return bod;
}

/**
@name:		isAtEnd
@purpose:	checks if the end of the token list has been reached
@param:		null
@return:	bool
*/
bool Parser::isAtEnd()
{
	return current_ == inTokens_.size();
}

/**
@name:		advance
@purpose:	moves to the next token in the list
@param:		null
@return:	Token::pointer_type
*/
Token::pointer_type Parser::advance()
{
	if (!isAtEnd()) ++current_;
	return previous();
}

/**
@name:		peek
@purpose:	returns the current token in the list
@param:		null
@return:	Token::pointer_type
*/
Token::pointer_type Parser::peek()
{
	return inTokens_.at(current_);
}

/**
@name:		previous
@purpose:	returns the previous token in the list
@param:		null
@return:	Token::pointer_type
*/
Token::pointer_type Parser::previous()
{
	if (current_ > 0)
		return inTokens_.at(current_ - 1);

	return inTokens_.at(0);
}

/**
@name:		consume
@purpose:	advances the token list index, and throws an exception if the template type and the current token's type do not match
@param:		std::string
@return:	Token::pointer_type
*/
template<typename type>
Token::pointer_type Parser::consume(std::string msg)
{
	if (!isAtEnd() && is<type>(peek())) return advance();

	throw ParserException(msg);
}

/**
@name:		check
@purpose:	checks if the template type and the current token type match, then advances the token list index
@param:		null
@return:	Token::pointer_type
*/
template<typename T>
bool Parser::check()
{
	if (!isAtEnd() && is<T>(peek()))
	{
		advance();
		return true;
	}

	return false;
}

/**
@name:		expression
@purpose:	returns an assignment expression, or anything of lower precedence
@param:		null
@return:	Parser::expr_p
*/
Parser::expr_p Parser::expression()
{
	return assignment();
}

/**
@name:		assignment
@purpose:	assigns a value or reference to a variable or get expression
@param:		null
@return:	Parser::expr_p
*/
Parser::expr_p Parser::assignment()
{
	expr_p exp = logOr();

	if (check<Assignment>())
	{
		Token::pointer_type oper = previous();
		expr_p val = assignment();

		if (is<VariableExpression>(exp))
			return expr_p(new AssignExpression(convert<VariableExpression>(exp)->oper_, val));
		else if (is<GetExpression>(exp))
		{
			GetExpression::pointer_type get = convert<GetExpression>(exp);
			return expr_p(new SetExpression(get->obj_, get->name_, val));
		}

		stringstream msg;
		msg << "Variable \"" << convert<VariableExpression>(exp)->oper_->getName() << "\" does not exist.";
		throw ParserException(msg.str());
	}

	return exp;
}

/**
@name:		logOr
@purpose:	performs a logical Or operation
@param:		null
@return:	Parser::expr_p
*/
Parser::expr_p Parser::logOr()
{
	expr_p expr = logAnd();

	while (check<Or>())
	{
		Token::pointer_type oper = previous();
		expr_p right = logAnd();
		expr.reset(new LogicalExpression(expr, convert<BinaryOperator>(oper), right));
	}
	
	return expr;
}

/**
@name:		logAnd
@purpose:	performs a logical And operation
@param:		null
@return:	Parser::expr_p
*/
Parser::expr_p Parser::logAnd()
{
	expr_p expr = equality();

	while (check<And>())
	{
		Token::pointer_type oper = previous();
		expr_p right = equality();
		expr.reset(new LogicalExpression(expr, convert<BinaryOperator>(oper), right));
	}

	return expr;
}

/**
@name:		equality
@purpose:	performs and equality or inequality operation on two expression
@param:		null
@return:	Parser::expr_p
*/
Parser::expr_p Parser::equality()
{
	expr_p exp = comparison();
	
	while (check<Inequality>() || check<Equality>())
	{
		advance();
		Token::pointer_type oper = previous();
		expr_p right = comparison();
		exp.reset(new BinaryExpression(exp, convert<BinaryOperator>(oper), right));
	}

	return exp;
}

/**
@name:		comparison
@purpose:	performs and comparison operation on two expressions, excluding inequality and equality
@param:		null
@return:	Parser::expr_p
*/
Parser::expr_p Parser::comparison()
{
	expr_p exp = addition();

	while (check<Greater>() || check<GreaterEqual>() || check<Less>() || check<LessEqual>())
	{
		Token::pointer_type oper = previous();
		expr_p right = addition();
		exp.reset(new BinaryExpression(exp, convert<BinaryOperator>(oper), right));
	}

	return exp;
}

/**
@name:		addition
@purpose:	adds two expression
@param:		null
@return:	Parser::expr_p
*/
Parser::expr_p Parser::addition()
{
	expr_p exp = multiplication();

	while (check<Subtraction>() || check<Addition>())
	{
		Token::pointer_type oper = previous();
		expr_p right = multiplication();
		exp.reset(new BinaryExpression(exp, convert<BinaryOperator>(oper), right));
	}

	return exp;
}

/**
@name:		multiplication
@purpose:	multiplies two expression
@param:		null
@return:	Parser::expr_p
*/
Parser::expr_p Parser::multiplication()
{
	expr_p exp = unary();

	while (check<Division>() || check<Multiplication>())
	{
		Token::pointer_type oper = previous();
		expr_p right = unary();
		exp.reset(new BinaryExpression(exp, convert<BinaryOperator>(oper), right));
	}

	return exp;
}

/**
@name:		unary
@purpose:	Perfomrs a unary operation on an expression
@param:		null
@return:	Parser::expr_p
*/
Parser::expr_p Parser::unary()
{
	if (check<Negation>() || check<Identity>())
	{
		Token::pointer_type oper = previous();
		expr_p right = unary();
		return expr_p(new UnaryExpression(convert<UnaryOperator>(oper), right));
	}

	return call();
}

/**
@name:		call
@purpose:	Creates a new function call, or gets a reference to an object property
@param:		null
@return:	Parser::expr_p
*/
Parser::expr_p Parser::call()
{
	expr_p exp = primary();

	while (true)
	{
		if (check<LeftBracket>())
			exp = finishCall(exp);
		else if (check<Dot>())
		{
			Variable::pointer_type name = convert<Variable>(consume<Variable>("Expected property name after '.'"));
			exp.reset(new GetExpression(exp, name));
		}
		else
			break;
	}

	return exp;
}

/**
@name:		primary
@purpose:	Returns a literal, variable, or "super" reference
@param:		null
@return:	Parser::expr_p
*/
Parser::expr_p Parser::primary()
{
	if (check<False>())
		return expr_p(new LiteralExpression(LiteralExpression::oper_type(new False())));

	if (check<True>())
		return expr_p(new LiteralExpression(LiteralExpression::oper_type(new True())));

	if (check<Number>() || check<Word>())
		return expr_p(new LiteralExpression(convert<Operand>(previous())));

	if (check<Variable>())
		return expr_p(new VariableExpression(convert<Variable>(previous())));

	if (check<Me>())
		return expr_p(new MeExpression(Variable::pointer_type(new Variable("me"))));

	if (check<Super>())
	{
		Variable::pointer_type keyword(new Variable("super"));
		consume<Dot>("Expected '.' after 'super'.");
		Variable::pointer_type method = convert<Variable>(consume<Variable>("Expected superclass method name."));
		return expr_p(new SuperExpression(keyword, method));
	}

	if (check<LeftBracket>())
	{
		expr_p exp = expression();
		consume<RightBracket>("Expected ')' after expression");
		return expr_p(new GroupExpression(exp));
	}

	throw ParserException("Keyword/operator is missing an operand.");
}

/**
@name:		finishCall
@purpose:	parses the arguments for a function call
@param:		expr_p
@return:	Parser::expr_p
*/
Parser::expr_p Parser::finishCall(expr_p callee)
{
	list<expr_p> args;
	
	if (!isAtEnd() && !is<RightBracket>(peek()))
	{
		do
			args.push_back(expression());
		while(check<Comma>());
	}

	return expr_p(new CallExpression(callee, convert<RightBracket>(consume<RightBracket>("Expected ')' after function call.")), args));
}
