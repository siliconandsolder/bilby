#include "parser.hpp"
#include "lexer.hpp"
#include "interpreter.hpp"
#include "resolver.hpp"
#include <iostream>
using namespace std;

int main()
{	
	try
	{
		Lexer lex;
		TokenList tokens = lex.analyze("class test {data: let a; methods: doSomething(){ shout me.a; }} let t = test(); t.a = 2; t.doSomething();");

		Parser p(tokens);
		Parser::stmt_list lst = p.parse();

		Interpreter it;
		
		Resolver res(&it);
		res.resolve(lst);

		it.interpret(lst);
	}
	catch (exception &e)
	{
		cout << e.what() << endl;
	}
}