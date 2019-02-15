/**	@file word.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif defines the structure of a String-type class
*/

#pragma once
#include "token.hpp"

class Word : public Operand
{
public:
	DEF_POINTER_TYPE(Word)
	DEF_IS_CONVERTIBLE_FROM(Word)
	using value_type = string_type;
private:
	value_type value_;
public:
	Word(value_type value = "") : value_(value) {}
	value_type getValue() const { return value_; }
	string_type toString() const { return value_; }
};