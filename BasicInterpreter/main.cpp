#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"
#include "resolver.hpp"
#include "interpreter.hpp"

using namespace std;

int main()
{
	try
	{
		Lexer lex;
		TokenList list = lex.analyze("float x = abs(-1); shout x;");
		Parser parse(list);
		Parser::stmt_list stmts = parse.parse();

		Interpreter interpreter;
		Resolver resolver(&interpreter);
		resolver.resolve(stmts);
		interpreter.interpret(stmts);
	}
	catch (exception& ex)
	{
		cerr << ex.what() << endl;
	}
}