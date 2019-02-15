/**	@file token.cpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of Token class
*/

#include "token.hpp"

/**
@name:		toString
@purpose:	returns the object type as a string
@param:		null
@return:	string_type
*/
Token::string_type Token::toString() const
{
	return string_type("<") + string_type(typeid(*this).name()).substr(6) + string_type(">");
}
