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
		TokenList tokens = lex.analyze("class doughnut { cook() { shout \"fry until brown\"; } } class boston inherits doughnut { cook() { super.cook(); shout \"pipe full of custard.\"; } } boston().cook();");

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