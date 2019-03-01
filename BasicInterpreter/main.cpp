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
		TokenList tokens = lex.analyze("class testclass {data: int ab; methods: object init() { me.ab = 2; return; } int tester(int a) { return me.ab + a; } void shouter() { shout \"hello!\"; }} object t = testclass(); int d = t.tester(2); shout d; t.shouter();");

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