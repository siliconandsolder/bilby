#include "boolean.hpp"
#include <boost/lexical_cast.hpp>

/**
@name:		toString
@purpose:	returns the value of the bool as a string
@param:		null
@return:	string_type
*/
Boolean::string_type Boolean::toString() const
{
	return boost::lexical_cast<string_type>(getValue());
}
