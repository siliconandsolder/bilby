/**	@file boolean.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif defines the structure of the boolean operand
*/

#pragma once
#include "operand.hpp"

class Boolean : public Operand
{
public:
	DEF_POINTER_TYPE(Boolean)
	using value_type = bool;
private:
	value_type value_;
public:
	Boolean(value_type value = false) : value_(value) {}
	value_type getValue() const { return value_; }
	string_type toString() const override;
};

class True : public Boolean
{
public:
	True() : Boolean(true) {}
	DEF_IS_CONVERTIBLE_FROM(True)
};

class False : public Boolean
{
public:
	False() : Boolean(false) {}
	DEF_IS_CONVERTIBLE_FROM(False)
};