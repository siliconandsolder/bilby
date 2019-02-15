#include "float.hpp"
#include <boost/lexical_cast.hpp>

/**
@name:		toString
@purpose:	returns the value of a Float as a string
@param:		null
@return:	string_type
*/
Float::string_type Float::toString() const
{
	return boost::lexical_cast<string_type>(getValue());
}
