#include "parser.hpp"
#include "tokenizer.hpp"
#include "interpreter.hpp"
#include "resolver.hpp"
#include <iostream>
using namespace std;

int main()
{	
	try
	{
		Tokenizer tok;
		TokenList tokens = tok.tokenize("class doughnut { cook() { shout \"fry until brown\"; } } class boston inherits doughnut { cook() { super.cook(); shout \"pipe full of custard.\"; } } boston().cook();");

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