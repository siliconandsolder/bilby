/**	@file boolean.cpp
	@author Garth Santor, modified by Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif implementation of Boolean
*/

#include "boolean.hpp"
#include <boost/lexical_cast.hpp>
#include <sstream>
using namespace std;

/**
@name:		toString
@purpose:	returns the value of the bool as a string
@param:		null
@return:	string_type
*/
Boolean::string_type Boolean::toString() const
{
	ostringstream oss;
	oss << std::boolalpha << value_;
	return oss.str();
}
