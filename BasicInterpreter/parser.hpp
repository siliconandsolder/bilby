/**	@file parser.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif defines the structure of the language parser
*/

#pragma once
#include "visitor.hpp"
#include <list>

class Parser
{
public:
	class ParserException : public std::exception
	{
	private:
		std::string msg_;
	public:
		ParserException() = delete;
		ParserException(std::string msg) : std::exception(msg.c_str()) {}
	};

public:
	using expr_p = Expression::expr_type;
	using stmt_p = Statement::pointer_type;
	using stmt_list = std::list<stmt_p>;
private:
	TokenList inTokens_;
	unsigned current_;
public:
	Parser() = delete;
	Parser(TokenList const& infixTokens) : inTokens_(infixTokens), current_(0) {}
	stmt_list parse();

private:
	bool isAtEnd();
	/*template<typename... types>
	bool match();*/
	Token::pointer_type advance();
	Token::pointer_type peek();
	Token::pointer_type previous();
	template<typename type>
	Token::pointer_type consume(std::string msg);
	
	stmt_p statement();
	stmt_p classStatement();
	stmt_p printStatement();
	stmt_p retStatement();
	stmt_p expStatement();
	stmt_p varDeclaration();
	stmt_p funcStatement(std::string kind, Variable::pointer_type name);
	stmt_p block();
	stmt_p ifStatement();
	stmt_p whileStatement();
	stmt_p doWhileStatement();
	stmt_p forStatement();

	VarType assignVarType(Token::pointer_type tok);

	// expression code
	expr_p expression();
	expr_p assignment();
	expr_p logOr();
	expr_p logAnd();
	expr_p equality();
	expr_p comparison();
	expr_p addition();
	expr_p multiplication();
	expr_p unary();
	expr_p call();
	expr_p primary();

	// helper functions
	expr_p finishCall(expr_p callee);
	template<typename T>
	bool check();
};

//template<typename... types>
//inline bool Parser::match()
//{
//	if constexpr(sizeof...(types) == 1)
//		return is<types>(peek());
//	else
//		return is<types>(peek()) || match<types>();
//}
