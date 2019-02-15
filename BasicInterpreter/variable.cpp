#include "variable.hpp"
#include <boost/lexical_cast.hpp>

/**
@name:		toString
@purpose:	returns the name of a variable as a string
@param:		null
@return:	string_type
*/
Variable::string_type Variable::toString() const
{
	return name_;
}
