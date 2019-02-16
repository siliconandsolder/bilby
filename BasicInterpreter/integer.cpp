/**	@file integer.cpp
	@author Garth Santor, modified by Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of Integer class
*/

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
