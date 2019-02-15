/**	@file math_func.cpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of miscellaneous functions
*/

#include "misc_func.hpp"
#include <ctime>
#include <chrono>
using namespace std;

/**
@name:		call
@purpose:	Returns the current date and time as a word
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type DateTime::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	auto tp = chrono::system_clock::now();

	time_t now = chrono::system_clock::to_time_t(tp);
	string sTime = ctime(&now);

	return tok_p(make<Word>(sTime));
}

/**
@name:		call
@purpose:	Returns the current UTC date and time as a word
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type DateTimeUTC::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	auto tp = chrono::system_clock::now();

	time_t now = chrono::system_clock::to_time_t(tp);
	auto utc = gmtime(&now);
	string sTime = asctime(utc);

	return tok_p(make<Word>(sTime));
}

/**
@name:		call
@purpose:	Returns the number of second since the Epoch as an Integer
@param:		Interpreter *, std::list<Token::pointer_type>
@return:	Token::pointer_type
*/
Token::pointer_type Epoch::call(Interpreter * interpreter, std::list<Token::pointer_type> args)
{
	auto tp = chrono::system_clock::now();

	auto epoch = chrono::duration_cast<chrono::seconds>(tp.time_since_epoch()).count();
	auto seconds = static_cast<Integer::value_type>(epoch);

	return tok_p(make<Integer>(seconds));
}
