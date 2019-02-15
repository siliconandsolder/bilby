#include "parser.hpp"
#include "operand.hpp"
#include "operator.hpp"
#include "tokenizer.hpp"
#include "keywords.hpp"
#include "integer.hpp"
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

Parser::stmt_p Parser::declaration()
{
	if (is<Let>(peek())) return varDeclaration();

	return statement();
}

/**
@name:		statement
@purpose:	returns a statement pointer based on the opening keyword
@param:		null
@return:	Parser::stmt_p
*/
Parser::stmt_p Parser::statement()
{
	if (is<If>(peek())) return ifStatement();
	else if (is<Class>(peek())) return classStatement();
	else if (is<Func>(peek())) return funcStatement("function");
	else if (is<For>(peek())) return forStatement();
	else if (is<While>(peek())) return whileStatement();
	else if (is<Do>(peek())) return doWhileStatement();
	else if (is<Print>(peek())) return printStatement();
	else if (is<Return>(peek())) return retStatement();
	else if (is<Let>(peek())) return varDeclaration();
	else if (is<LeftBrace>(peek())) return block();

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
	advance();
	Variable::pointer_type name = convert<Variable>(consume<Variable>("Expected class name."));
	
	VariableExpression::pointer_type super = nullptr;
	if (check<Inherits>())
	{
		consume<Variable>("Expected superclass name.");
		super.reset(new VariableExpression(convert<Variable>(previous())));
	}
	
	consume<LeftBrace>("Expected '{' before class body.");

	list<StmtFunc::pointer_type> methods;
	while (!isAtEnd() && !is<RightBrace>(peek()))
	{
		--current_;
		methods.push_back(convert<StmtFunc>(funcStatement("method")));
	}
		

	consume<RightBrace>("Expected closing brace after class definition.");

	return stmt_p(new StmtClass(name, super, methods));
}

/**
@name:		printStatement
@purpose:	creates a new print statement
@param:		null
@return:	Parser::stmt_p
*/
Parser::stmt_p Parser::printStatement()
{
	advance();
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
	advance();
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
	advance();
	Token::pointer_type var = consume<Variable>("Expected a variable name.");
	
	expr_p init;
	if (is<Assignment>(peek()))
	{
		advance();
		init = expression();
	}	

	consume<SemiColon>("Expected ';'");
	stmt_p expr(new StmtVariable(convert<Variable>(var), init));
	return expr;
}

/**
@name:		funcStatement
@purpose:	creates a new function statement with a name, parameters, and a block statement
@param:		null
@return:	Parser::stmt_p
*/
Parser::stmt_p Parser::funcStatement(std::string kind)
{
	advance();
	Variable::pointer_type name = convert<Variable>(consume<Variable>("Expected " + kind + " name"));

	consume<LeftBracket>("Expected '(' after function name");
	
	list<Token::pointer_type> params;
	if (!is<RightBracket>(peek()))
	{
		do
		{
			if (params.size() == 8)
				throw ParserException("Cannot have more than 8 paramaters.");

			consume<Let>("Parameters must start with 'let' identifier.");

			params.push_back(consume<Variable>("Expected paramater name."));
		}
		while (check<Comma>());
	}

	consume<RightBracket>("Expected ')' after function parameters");
	consume<LeftBrace>("Expected '{' after function header");
	
	stmt_list body;

	while (!is<RightBrace>(peek()) && !isAtEnd())
		body.push_back(statement());

	consume<RightBrace>("Expected '}'");

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
	advance();
	stmt_list statements;

	while (!is<RightBrace>(peek()) && !isAtEnd())
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
	advance();
	consume<LeftBracket>("Expected '(' after 'if'");
	expr_p cond = expression();
	consume<RightBracket>("Expected ')' after if condition.");

	stmt_p thenB = statement();
	stmt_p elseB = nullptr;
	if (!isAtEnd() && is<Else>(peek()))
	{
		advance();
		elseB = statement();
	}
		
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
	advance();
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
	advance();
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
	advance();
	consume<LeftBracket>("Expected '(' after 'for'");
	
	stmt_p init;
	if (is<SemiColon>(peek()))
		init = nullptr;
	else if (is<Let>(peek()))
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
@name:		check
@purpose:	checks if the end of the token list has been reached, and if the parameter matches the type of the current token
@param:		Token::pointer_type
@return:	bool
*/
bool Parser::check(Token::pointer_type type)
{
	if (isAtEnd()) return false;
	return peek() == type;
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

void Parser::synchronize()
{
	advance();

	while (!isAtEnd())
	{
		if (is<For>(peek()) || is<Print>(peek()) || is<Let>(peek()))
			return;

		advance();
	}
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

	if (!isAtEnd() && is<Assignment>(peek()))
	{
		advance();
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

	if (!isAtEnd() && (is<Or>(peek())))
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

	if (!isAtEnd() && (is<And>(peek())))
	{
		advance();
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
	
	while (!isAtEnd() && (is<Inequality>(peek()) || is<Equality>(peek())))
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

	while (!isAtEnd() && (is<Greater>(peek()) || is<GreaterEqual>(peek()) || is<Less>(peek()) || is<LessEqual>(peek())))
	{
		advance();
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

	while (!isAtEnd() && (is<Subtraction>(peek()) || is<Addition>(peek())))
	{
		advance();
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

	while (!isAtEnd() && (is<Division>(peek()) || is<Multiplication>(peek())))
	{
		advance();
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
	if (!isAtEnd() && (is<Negation>(peek()) || is<Identity>(peek())))
	{
		advance();
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
		if (!isAtEnd() && is<LeftBracket>(peek()))
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
	if (!isAtEnd() && is<False>(peek()))
	{
		advance();
		return expr_p(new LiteralExpression(LiteralExpression::oper_type(new False())));
	}

	if (!isAtEnd() && is<True>(peek()))
	{
		advance();
		return expr_p(new LiteralExpression(LiteralExpression::oper_type(new True())));
	}

	if (!isAtEnd() && (is<Number>(peek()) || is<Word>(peek())))
	{
		advance();
		return expr_p(new LiteralExpression(convert<Operand>(previous())));
	}

	if (!isAtEnd() && is<Variable>(peek()))
	{
		advance();
		return expr_p(new VariableExpression(convert<Variable>(previous())));
	}

	if (!isAtEnd() && is<Me>(peek()))
	{
		advance();
		return expr_p(new MeExpression(Variable::pointer_type(new Variable("me"))));
	}

	if (!isAtEnd() && is<Super>(peek()))
	{
		advance();
		Variable::pointer_type keyword(new Variable("super"));
		consume<Dot>("Expected '.' after 'super'.");
		Variable::pointer_type method = convert<Variable>(consume<Variable>("Expected superclass method name."));
		return expr_p(new SuperExpression(keyword, method));
	}

	if (!isAtEnd() && (is<LeftBracket>(peek())))
	{
		advance();
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
	advance();
	list<expr_p> args;
	
	if (!isAtEnd() && !is<RightBracket>(peek()))
	{
		do
		{
			args.push_back(expression());
		}
		while(check<Comma>());
	}

	return expr_p(new CallExpression(callee, convert<RightBracket>(consume<RightBracket>("Expected ')' after function call.")), args));
}
