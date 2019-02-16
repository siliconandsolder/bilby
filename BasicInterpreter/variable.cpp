/**	@file variable.cpp
	@author Garth Santor, modified by Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of Variable class
*/

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
