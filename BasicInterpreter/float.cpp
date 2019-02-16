/**	@file float.cpp
	@author Garth Santor, modified by Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of Float class
*/

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
