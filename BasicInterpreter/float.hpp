#pragma once

#include "number.hpp"
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/math/constants/constants.hpp>

class Float : public Number
{
public:
	DEF_POINTER_TYPE(Float)
	using value_type = boost::multiprecision::number < boost::multiprecision::cpp_dec_float<1000, int32_t, void> >;
private:
	value_type value_;
public:
	Float(value_type value = 0.0) : value_(value) {}

	value_type getValue() const { return value_; }
	string_type toString() const override;
};

class Pi : public Float
{
public:
	Pi() : Float(boost::math::constants::pi<value_type>()) {}

	DEF_IS_CONVERTIBLE_FROM(Pi)
};

class E : public Float
{
public:
	E() : Float(boost::math::constants::e<value_type>()) {}

	DEF_IS_CONVERTIBLE_FROM(E)
};