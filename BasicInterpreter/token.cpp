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
