/**	@file operand.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif distinguishes numeric operands from non-numeric
*/

#pragma once
#include "operand.hpp"

class Number : public Operand
{
	DEF_POINTER_TYPE(Number)
};