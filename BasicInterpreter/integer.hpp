/**	@file integer.hpp
	@author Benjamin Godin
	@date 2019-02-14
	@version 1.0.0
	@note Developed for C++17
	@breif defines the structure of the integer operand
*/

#pragma once
#include "number.hpp"
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/debug_adaptor.hpp>

class Integer : public Number
{
public:
	using value_type = boost::multiprecision::cpp_int;
	DEF_POINTER_TYPE(Integer)
private:
	value_type value_;
public:
	Integer(value_type value = 0) : value_(value) {}

	value_type getValue() const { return value_; }
	string_type toString() const override;

	DEF_IS_CONVERTIBLE_FROM(Integer)
};