#include "integer.hpp"
#include <boost/lexical_cast.hpp>

/**
@name:		toString
@purpose:	returns the value of a Integer as a string
@param:		null
@return:	string_type
*/
Integer::string_type Integer::toString() const
{
	return boost::lexical_cast<string_type>(getValue());
}
